import { Injectable, HostBinding, EventEmitter, Output } from '@angular/core';
import { HttpClient } from '@angular/common/http';

import { ProgressWindow } from '../pages/generator/progressWindow/progressWindow.component';

import * as post from 'post-robot';

@Injectable()
export class GUIGlobal {

  //Globals for GUI HTML
  public generator_tabsVisibilityMap: Object = {};
  public generator_settingsVisibilityMap: Object = {};

  public generator_settingsMap: Object = {};
  public generator_customColorMap: Object = {};

  public generator_presets: Object = {};

  globalVars: Map<string, any>;
  electronEvents: Array<any> = [];

  @HostBinding('class.indigo-pink') materialStyleIndigo: boolean = true;

  @Output() globalEmitter: EventEmitter<object> = new EventEmitter();
  
  constructor(private http: HttpClient) {
    this.globalVars = new Map<string, any>([
      ["appReady", false],
      ["appType", null],
      ["electronAvailable", false],
      ["webSourceVersion", ""],
      ["webIsMasterVersion", false],
      ["generatorSettingsArray", []],
      ["generatorSettingsObj", {}],
      ["generatorCosmeticsArray", []],
      ["generatorCosmeticsObj", {}],
      ["generatorGoalDistros", []]
    ]);
  }

  globalInit(appType: string) {

    if (!appType)
      appType = "generator"; //Generator is the default

    console.log("APP Type:", appType);
    this.setGlobalVar("appType", appType);

    if ((<any>window).electronAvailable) { //Electron/Offline mode

      if ((<any>window).apiPythonSourceFound) {
        this.setGlobalVar("electronAvailable", true);
        console.log("Electron API available");
        console.log("Running on OS:", (<any>window).apiPlatform);

        this.createElectronEvents();
        this.electronInit();
      }
      else {
        console.error("Improper setup, exit GUI");
      }
    }
    else { //Online/Web mode
      console.log("Electron API not available, assume web mode");

      if (!(<any>window).pythonSourceVersion) {
        console.error("No python version defined, exit");
        return;
      }

      this.setGlobalVar("webSourceVersion", (<any>window).pythonSourceVersion);

      if ((<any>window).pythonSourceIsMasterVersion)
        this.setGlobalVar("webIsMasterVersion", (<any>window).pythonSourceIsMasterVersion);

      console.log("Web version: " + (<any>window).pythonSourceVersion + " ; master version:", this.getGlobalVar("webIsMasterVersion"));

      this.webInit();
    }
  }

  ngOnDestroy() {
    if ((<any>window).electronAvailable) {
      this.destroyElectronEvents();
    }
  }

  createElectronEvents() { //Electron only

    var self = this;

    let maximizedEvent = post.on('window-maximized', function (event) {
      let res = event.data;

      if (res == true)
        self.globalEmitter.emit({ name: "window_maximized" });
      else
        self.globalEmitter.emit({ name: "window_unmaximized" });
    });

    this.electronEvents.push(maximizedEvent);
  }

  createWebEvents() { //Web only

    var self = this;

    (<any>window).addEventListener('emscripten_cache_file_found', function (event) {

      let detail = event.detail;

      //Update settings entry for cached file and then refresh GUI
      if (detail) {

        if (detail.name == "ROM")
          self.generator_settingsMap["rom"] = "<using cached ROM>";
        else if (detail.name == "WAD")
          self.generator_settingsMap["web_wad_file"] = "<using cached WAD>";
        else if (detail.name == "COMMONKEY")
          self.generator_settingsMap["web_common_key_file"] = "<using cached common key>";

        self.globalEmitter.emit({ name: "refresh_gui" });
      }

    }, false);
  }

  destroyElectronEvents() {
    this.electronEvents.forEach(postRobotEvent => {
      postRobotEvent.cancel();
    });
  }

  electronInit() {
    this.electronLoadGeneratorGUISettings().then(() => {
      this.setGlobalVar("appReady", true);
      this.globalEmitter.emit({ name: "init_finished" });
    }).catch(err => {
      console.error("exit due error:", err);
    });
  }

  webInit() {
    this.webLoadGeneratorGUISettings().then(() => {
      this.setGlobalVar("appReady", true);
      this.globalEmitter.emit({ name: "init_finished" });
    }).catch(err => {
      console.error("exit due error:", err);
    });
  }

  setGlobalVar(key: string, value: any) {
    if (this.globalVars.has(key)) {
      this.globalVars.set(key, value);
    }
  }

  getGlobalVar(key: string) {

    if (this.globalVars.has(key))
      return this.globalVars.get(key);

    return "error";
  }

  copyToClipboard(content: string) {

    if (this.getGlobalVar('electronAvailable')) { //Electron
      post.send(window, 'copyToClipboard', { content: content }).then(event => {
        console.log('copied to clipboard');
      }).catch(err => {
        console.error(err);
      });
    }
    else { //Web
      let selBox = document.createElement('textarea');
      selBox.style.position = 'fixed';
      selBox.style.left = '0';
      selBox.style.top = '0';
      selBox.style.opacity = '0';
      selBox.value = content;

      document.body.appendChild(selBox);
      selBox.focus();
      selBox.select();
      document.execCommand('copy');

      document.body.removeChild(selBox);
    }
  }

  async browseForFile(fileTypes: any) { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    let event = await post.send(window, 'browseForFile', { fileTypes: fileTypes });
    let res = event.data;

    if (!res || res.length != 1 || !res[0] || res[0].length < 1)
      throw Error("dialog_cancelled");

    return res[0];
  }

  async browseForDirectory() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    let event = await post.send(window, 'browseForDirectory');
    let res = event.data;

    if (!res || res.length != 1 || !res[0] || res[0].length < 1)
      throw Error("dialog_cancelled");

    return res[0];
  }

  async createAndOpenPath(path: string) { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    let event = await post.send(window, 'createAndOpenPath', path);
    let res = event.data;

    if (res == true)
      return true;
    else
      throw Error("path_not_opened");
  }

  minimizeWindow() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      return;

    post.send(window, 'window-minimize').then(() => {
      console.log('window minimized');
    }).catch(err => {
      console.error(err);
    });
  }

  maximizeWindow() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      return;

    post.send(window, 'window-maximize').then(() => {
      console.log('window maximize state updated');
    }).catch(err => {
      console.error(err);
    });
  }

  async isWindowMaximized() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    try {
      let event = await post.send(window, 'window-is-maximized');
      let res = event.data;

      //console.log("window maximized state:", res);
      return res;
    }
    catch (err) {
      console.error(err);
      throw Error(err);
    }
  }

  closeWindow() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      return;

    this.saveCurrentSettingsToFile();

    setTimeout(() => {

      post.send(window, 'window-close').then(() => {
        console.log('window closed');
      }).catch(err => {
        console.error(err);
      });
    });
  }

  async electronLoadGeneratorGUISettings() {

    var guiSettings = post.send(window, 'getGeneratorGUISettings');
    var lastUserSettings = post.send(window, 'getGeneratorGUILastUserSettings');

    try {
      var results = await Promise.all([guiSettings, lastUserSettings]);

      let res = JSON.parse(results[0].data);
      let userSettings = results[1].data;

      if (userSettings)
        userSettings = JSON.parse(userSettings);

      this.parseGeneratorGUISettings(res, userSettings);

    } catch (err) {
      console.error(err);
      throw Error(err);
    }
  }

  async webLoadGeneratorGUISettings() {

    var res = null;
    var userSettings = null;

    //Get generator settings
    if ((<any>window).webGeneratorSettingsMap) { //Master versions on the web pre-define a settings map to use before Angular will load
      res = (<any>window).webGeneratorSettingsMap;
      console.log("Settings map is available from the DOM");
    }
    else { //Dev versions on the web request a pre-built settings map from the server at runtime

      let url = (<any>window).location.protocol + "//" + (<any>window).location.host + "/angular/dev/" + this.getGlobalVar("webSourceVersion").replace(/ /g, "_") + "/utils/settings_list.json";
      console.log("Settings map is not available. Request it:", url);

      res = await this.http.get(url, { responseType: "json" }).toPromise();
    }

    //Get last user settings from browser cache for the appropriate app
    try {
      userSettings = localStorage.getItem(this.getGlobalVar("appType") == "generator" ? "generatorSettings_" + this.getGlobalVar("webSourceVersion") : "patcherSettings_" + this.getGlobalVar("webSourceVersion"));

      if (userSettings && userSettings.length > 0) {
        userSettings = JSON.parse(userSettings);
      }
      else { //Try the opposite app type to see if a cached settings map is available there
        userSettings = localStorage.getItem(this.getGlobalVar("appType") == "generator" ? "patcherSettings_" + this.getGlobalVar("webSourceVersion") : "generatorSettings_" + this.getGlobalVar("webSourceVersion"));

        if (userSettings && userSettings.length > 0) {
          userSettings = JSON.parse(userSettings);
        }
      }
    } catch (err) {
      console.error("Local storage not available");
    }

    //Get user presets if appType = generator
    if (this.getGlobalVar("appType") == "generator") {

      let userPresets = null;

      try {
        userPresets = localStorage.getItem("generatorPresets_" + this.getGlobalVar("webSourceVersion"));
      } catch { }

      if (userPresets && userPresets.length > 0) {

        userPresets = JSON.parse(userPresets);

        let adjustedUserPresets = {};

        //Tag user presets appropiately
        Object.keys(userPresets).forEach(presetName => {
          if (!(presetName in res.presets))
            adjustedUserPresets[presetName] = { settings: userPresets[presetName] };
        });

        Object.assign(res.presets, adjustedUserPresets);       
      }
    }

    this.parseGeneratorGUISettings(res, userSettings);

    //Check for cached files and then create web events after
    if ((<any>window).emscriptenFoundCachedROMFile)
      this.generator_settingsMap["rom"] = "<using cached ROM>";

    if ((<any>window).emscriptenFoundCachedWADFile)
      this.generator_settingsMap["web_wad_file"] = "<using cached WAD>";

    if ((<any>window).emscriptenFoundCachedCommonKeyFile)
      this.generator_settingsMap["web_common_key_file"] = "<using cached common key>";

    this.createWebEvents();
  }

  parseGeneratorGUISettings(guiSettings, userSettings) {

    const isRGBHex = /[0-9A-Fa-f]{6}/;

    //Intialize settings maps
    for (let tabIndex = 0; tabIndex < guiSettings.settingsArray.length; tabIndex++) {
      let tab = guiSettings.settingsArray[tabIndex];

      //Skip tabs that don't belong to this app and delete them from the guiSettings
      if ("app_type" in tab && tab.app_type && tab.app_type.indexOf(this.getGlobalVar("appType")) == -1) {

        guiSettings.settingsArray.splice(tabIndex, 1);
        tabIndex--;

        let index = guiSettings.cosmeticsArray.findIndex(entry => entry.name == tab.name);
        if (index != -1)
          guiSettings.cosmeticsArray.splice(index, 1);

        delete guiSettings.settingsObj[tab.name];
        delete guiSettings.cosmeticsObj[tab.name];

        continue;
      }

      this.generator_tabsVisibilityMap[tab.name] = true;

      tab.sections.forEach(section => {
        section.settings.forEach(setting => {

          this.generator_settingsVisibilityMap[setting.name] = true;

          if (setting.type == "SearchBox" && userSettings && setting.name in userSettings) { //Special parsing for SearchBox data

            let valueArray = [];

            userSettings[setting.name].forEach(entry => {

              let optionEntry = setting.options.find(option => {
                if (option.name == entry)
                  return true;

                return false;
              });

              if (optionEntry)
                valueArray.push(optionEntry);
            });

            this.generator_settingsMap[setting.name] = valueArray;
          }
          else if (setting.type == "Combobox" && userSettings && setting.name in userSettings) { //Ensure combobox option exists before applying it (in case of outdated settings being loaded)

            if (section.is_colors && typeof (userSettings[setting.name]) == "string" && isRGBHex.test(userSettings[setting.name])) { //Custom Color is treated as an exception
              this.generator_settingsMap[setting.name] = userSettings[setting.name];
            }
            else {
              let optionEntry = setting.options.find(option => {
                if (option.name == userSettings[setting.name])
                  return true;

                return false;
              });

              this.generator_settingsMap[setting.name] = optionEntry ? userSettings[setting.name] : setting.default;
            }
          }
          else if (userSettings && setting.name in userSettings && (setting.type == "Scale" || setting.type == "Numberinput")) { //Validate numberic values again before applying them
            this.verifyNumericSetting(userSettings, setting, false);
            this.generator_settingsMap[setting.name] = userSettings[setting.name];
          }
          else { //Everything else
            this.generator_settingsMap[setting.name] = userSettings && setting.name in userSettings ? userSettings[setting.name] : setting.default;
          }

          //Color section handling
          if (section.is_colors) {
            if (typeof (this.generator_settingsMap[setting.name]) == "string" && isRGBHex.test(this.generator_settingsMap[setting.name])) { //Resolve Custom Color
              this.generator_customColorMap[setting.name] = "#" + this.generator_settingsMap[setting.name];
              this.generator_settingsMap[setting.name] = "Custom Color";
            }
            else {
              this.generator_customColorMap[setting.name] = "";
            }
          }
        });
      });
    }

    //Add GUI only options
    this.generator_settingsMap["settings_string"] = userSettings && "settings_string" in userSettings ? userSettings["settings_string"] : "";
    this.generator_settingsVisibilityMap["settings_string"] = true;

    console.log("JSON Settings Data:", guiSettings);
    console.log("Last User Settings:", userSettings);
    console.log("Final Settings Map", this.generator_settingsMap);
    console.log("Goal Hint Distros", guiSettings.distroArray);

    //Save settings after parsing them
    this.setGlobalVar('generatorSettingsArray', guiSettings.settingsArray);
    this.setGlobalVar('generatorSettingsObj', guiSettings.settingsObj);
    this.setGlobalVar('generatorCosmeticsArray', guiSettings.cosmeticsArray);
    this.setGlobalVar('generatorCosmeticsObj', guiSettings.cosmeticsObj);
    this.setGlobalVar('generatorGoalDistros', guiSettings.distroArray);

    this.generator_presets = guiSettings.presets;
  }

  async versionCheck() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    try {

      var event = await post.send(window, 'getCurrentSourceVersion');

      var res: string = event.data;
      var result = { hasUpdate: false, currentVersion: "", latestVersion: "" };

      if (res && res.length > 0) {

        console.log("Local:", res);
        result.currentVersion = res;

        this.globalEmitter.emit({ name: "local_version_checked", version: res });

        let branch = res.includes("Release") ? "release" : "Dev";
        var remoteFile = await this.http.get("https://raw.githubusercontent.com/TestRunnerSRL/OoT-Randomizer/" + branch + "/version.py", { responseType: "text" }).toPromise();

        let remoteVersion = remoteFile.substr(remoteFile.indexOf("'") + 1);
        remoteVersion = remoteVersion.substr(0, remoteVersion.indexOf("'"));

        console.log("Remote:", remoteVersion);
        result.latestVersion = remoteVersion;

        //Compare versions
        result.hasUpdate = this.isVersionNewer(remoteVersion, res);

        return result;  
      }
      else {
        return result;
      }
    }
    catch (err) {
      console.error(err);
      throw Error(err);
    }
  }

  isVersionNewer(newVersion: string, oldVersion: string) {

    //Strip away dev strings
    if (oldVersion.startsWith("dev") && oldVersion.includes("_"))
      oldVersion = oldVersion.split("_")[1];

    if (newVersion.startsWith("dev") && newVersion.includes("_"))
      newVersion = newVersion.split("_")[1];

    let oldSplit = oldVersion.replace('v', '').replace(' ', '.').split('.');
    let newSplit = newVersion.replace('v', '').replace(' ', '.').split('.');

    //Version is not newer if the new version doesn't satisfy the format
    if (newSplit.length < 3)
      return false;

    //Version is newer if the old version doesn't satisfy the format
    if (oldSplit.length < 3)
      return true;

    //Compare major.minor.revision
    if (Number(newSplit[0]) > Number(oldSplit[0])) {
      return true;
    }
    else if (Number(newSplit[0]) == Number(oldSplit[0])) {
      if (Number(newSplit[1]) > Number(oldSplit[1])) {
        return true;
      }
      else if (Number(newSplit[1]) == Number(oldSplit[1])) {
        if (Number(newSplit[2]) > Number(oldSplit[2])) {
          return true;
        }
      }
    }

    return false;
  }

  findSettingByName(settingName: string) {

    for (let tabIndex = 0; tabIndex < this.getGlobalVar('generatorSettingsArray').length; tabIndex++) {
      let tab = this.getGlobalVar('generatorSettingsArray')[tabIndex];

      for (let sectionIndex = 0; sectionIndex < tab.sections.length; sectionIndex++) {
        let section = tab.sections[sectionIndex];

        for (let settingIndex = 0; settingIndex < section.settings.length; settingIndex++) {
          let setting = section.settings[settingIndex];

          if (setting.name == settingName)
            return setting;
        }
      }
    }

    return false;
  }

  verifyNumericSetting(settingsFile: any, setting: any, syncToGlobalMap: boolean = false) {

    let settingValue: any = settingsFile[setting.name];
    let error = false;
    let didCast = false;

    if (typeof (settingValue) != "string" && typeof (settingValue) != "number") { //Can't recover, bad type
      error = true;
    }
    else if (typeof (settingValue) == "string") { //Try to cast it

      if (Number(parseInt(settingValue)) != Number(settingValue)) { //Cast failed, not numeric    
        error = true;
      }
      else {
        settingValue = parseInt(settingValue);
        didCast = true;
      }
    }

    if (!error) {

      //Min/Max check
      let settingMin: number = setting["min"];
      let settingMax: number = setting["max"];

      if (("min" in setting) && settingValue < settingMin) {

        settingsFile[setting.name]  = settingMin;

        if (syncToGlobalMap) { //Global too if needed and refresh GUI after to signal change
          this.generator_settingsMap[setting.name] = settingMin;
          this.globalEmitter.emit({ name: "refresh_gui" });
        }

        error = true;
      }
      else if (("max" in setting) && settingValue > settingMax) {

        settingsFile[setting.name]  = settingMax;

        if (syncToGlobalMap) { //Global too if needed and refresh GUI after to signal change
          this.generator_settingsMap[setting.name] = settingMax;
          this.globalEmitter.emit({ name: "refresh_gui" });
        }

        error = true;
      }

      if (!error && didCast) { //No error, but setting had to be casted from string, fix it
        settingsFile[setting.name] = settingValue;

        if (syncToGlobalMap) { //Global too if needed and refresh GUI after to signal change
          this.generator_settingsMap[setting.name] = settingValue;
          this.globalEmitter.emit({ name: "refresh_gui" });
        }
      }
    }
    else { //Critical error, reset local value to default
      settingsFile[setting.name] = setting.default;

      if (syncToGlobalMap) { //Global too if needed and refresh GUI after to signal change
        this.generator_settingsMap[setting.name] = setting.default;
        this.globalEmitter.emit({ name: "refresh_gui" });
      }
    }

    return error;
  }

  applySettingsObject(settingsObj) {

    if (!settingsObj)
      return;

    const isRGBHex = /[0-9A-Fa-f]{6}/;

    this.getGlobalVar('generatorSettingsArray').forEach(tab => {
      tab.sections.forEach(section => {
        section.settings.forEach(setting => {

          if (setting.name in settingsObj) {

            if (setting.type == "SearchBox") { //Special parsing for SearchBox data

              let valueArray = [];

              settingsObj[setting.name].forEach(entry => {

                let optionEntry = setting.options.find(option => {
                  if (option.name == entry)
                    return true;

                  return false;
                });

                if (optionEntry)
                  valueArray.push(optionEntry);
              });

              this.generator_settingsMap[setting.name] = valueArray;
            }
            else if (setting.type == "Combobox") { //Ensure combobox option exists before applying it (in case of outdated settings being loaded)

              let optionEntry = setting.options.find(option => {
                if (option.name == settingsObj[setting.name])
                  return true;

                return false;
              });

              if (optionEntry)
                this.generator_settingsMap[setting.name] = settingsObj[setting.name];
            }
            else if (setting.type == "Scale" || setting.type == "Numberinput") { //Validate numberic values again before applying them
              this.verifyNumericSetting(settingsObj, setting, false);
              this.generator_settingsMap[setting.name] = settingsObj[setting.name];
            }
            else if (setting.type == "MultipleSelect") { //Validate list types before applying them
              if (Array.isArray(settingsObj[setting.name]))
                this.generator_settingsMap[setting.name] = settingsObj[setting.name];
            }
            else { //Everything else
              this.generator_settingsMap[setting.name] = settingsObj[setting.name];
            }

            //Color section handling
            if (section.is_colors) {

              if (typeof (settingsObj[setting.name]) == "string" && isRGBHex.test(settingsObj[setting.name])) { //Resolve Custom Color
                this.generator_customColorMap[setting.name] = "#" + settingsObj[setting.name];
                this.generator_settingsMap[setting.name] = "Custom Color";
              }
              else {
                this.generator_customColorMap[setting.name] = "";
              }
            }
          }
        });
      });
    });
  }

  applyDefaultSettings() {

    let cleanSettings = this.createSettingsFileObject(false, true, !this.getGlobalVar('electronAvailable'));

    this.getGlobalVar('generatorSettingsArray').forEach(tab => {
      tab.sections.forEach(section => {
        section.settings.forEach(setting => {

          if (setting.name in cleanSettings) {
            this.generator_settingsMap[setting.name] = setting.default; 
          }
        });
      });
    });
  }

  deleteSettingsFromMapWithCondition(settingsMap: any, keyName: string, keyValue: any) {

    this.getGlobalVar("generatorSettingsArray").forEach(tab => {

      tab.sections.forEach(section => {
        section.settings.forEach(setting => {

          if (keyName in setting && setting[keyName] == keyValue) {
            delete settingsMap[setting.name];
          }
        });
      });
    });
  }

  createSettingsFileObject(includeFromPatchFileSettings: boolean = true, includeSeedSettingsOnly: boolean = false, sanitizeForBrowserCache: boolean = false, cancelWhenError: boolean = false) {

    let settingsFile: any = {};

    Object.assign(settingsFile, this.generator_settingsMap);

    //Add in custom colors
    Object.keys(this.generator_customColorMap).forEach(colorSettingName => {
      if (this.generator_customColorMap[colorSettingName].length > 0 && settingsFile[colorSettingName] === "Custom Color") {
        settingsFile[colorSettingName] = this.generator_customColorMap[colorSettingName].substr(1);
      }
    });

    let invalidSettingsList = [];

    //Resolve search box entries (name only)
    this.getGlobalVar("generatorSettingsArray").forEach(tab => {

      tab.sections.forEach(section => {
        section.settings.forEach(setting => {

          if (setting.type == "SearchBox") {

            let valueArray = [];

            settingsFile[setting.name].forEach(entry => {
              valueArray.push(entry.name);
            });

            settingsFile[setting.name] = valueArray;
          }
          else if (setting.type == "Scale" || setting.type == "Numberinput") { //Validate numberic values again before saving them

            let error = this.verifyNumericSetting(settingsFile, setting, true);

            if (error) { //Input could not be recovered, abort
              invalidSettingsList.push(setting.text);
            }       
          }     
        });
      });
    });

    //Abort if any invalid settings were found
    if (invalidSettingsList && invalidSettingsList.length > 0) {
      this.globalEmitter.emit({ name: "dialog_error", message: "Some invalid settings were detected and had to be reset! This can happen if you type too fast into an input box. Please try again. The following settings were affected: " + invalidSettingsList.join(", ") });

      if (cancelWhenError)
        return null;
    }

    //Delete keys the python source doesn't need
    delete settingsFile["presets"];
    delete settingsFile["open_output_dir"];
    delete settingsFile["open_python_dir"];
    delete settingsFile["generate_from_file"];

    //Delete fromPatchFile keys if mode is fromSeed
    if (!includeFromPatchFileSettings) {
      delete settingsFile["patch_file"];
      delete settingsFile["repatch_cosmetics"];

      //Web only keys
      if (!this.getGlobalVar('electronAvailable')) {
        delete settingsFile["web_persist_in_cache"];
      }
    }

    //Delete keys not included in the seed
    if (includeSeedSettingsOnly) {

      //Not mapped settings need to be deleted manually
      delete settingsFile["settings_string"];

      //Delete all shared = false keys from map since they aren't included in the seed
      this.deleteSettingsFromMapWithCondition(settingsFile, "shared", false);
    }

    //Delete keys the browser can't save (web only)
    if (sanitizeForBrowserCache) {

      //Delete all settings of type Fileinput/Directoryinput. File objects can not be saved due browser sandbox
      this.deleteSettingsFromMapWithCondition(settingsFile, "type", "Fileinput");
      this.deleteSettingsFromMapWithCondition(settingsFile, "type", "Directoryinput");
    }

    return settingsFile;
  }

  saveCurrentSettingsToFile() {

    if (this.getGlobalVar('electronAvailable')) { //Electron
      post.send(window, 'saveCurrentSettingsToFile', this.createSettingsFileObject()).then(event => {
        console.log("settings saved to file");
      }).catch(err => {
        console.error(err);
      });
    }
    else { //Web
      try {
        localStorage.setItem(this.getGlobalVar("appType") == "generator" ? "generatorSettings_" + this.getGlobalVar("webSourceVersion") : "patcherSettings_" + this.getGlobalVar("webSourceVersion"), JSON.stringify(this.createSettingsFileObject(true, false, true)));
      } catch { }
    }
  }

  convertSettingsToString() {
    var self = this;

    return new Promise<string>(function (resolve, reject) {

      if (self.getGlobalVar('electronAvailable')) { //Electron

        post.send(window, 'convertSettingsToString', self.createSettingsFileObject(false, true)).then(event => {

          var listenerSuccess = post.once('convertSettingsToStringSuccess', function (event) {

            listenerError.cancel();

            let data = event.data;
            resolve(data);
          });

          var listenerError = post.once('convertSettingsToStringError', function (event) {

            listenerSuccess.cancel();

            let data = event.data;

            console.error("[convertSettingsToString] Python Error:", data);
            reject(data);
          });

        }).catch(err => {
          console.error("[convertSettingsToString] Post-Robot Error:", err);
          reject(err);
        });
      }
      else { //Web
        let url = (<any>window).location.protocol + "//" + (<any>window).location.host + "/settings/parse?version=" + self.getGlobalVar("webSourceVersion");
        console.log("Request string from:", url);

        self.http.post(url, JSON.stringify(self.createSettingsFileObject(false, true, true)), { responseType: "text", headers: { "Content-Type": "application/json" } }).toPromise().then(res => {
          resolve(res);
        }).catch(err => {
          console.error("[convertSettingsToString] Web Error:", err);
          reject(err);
        });
      }
    });
  }
  

  get_keyrings_string(key_rings_selection)
  {
    if(key_rings_selection === "")
    {
      return undefined;
    }
    var key_rings_list = key_rings_selection.split(",");
    
    if(key_rings_list.length == 9)
    {
      return "Small Key Rings";
    }
    const keyRingMap = {
      'Forest Temple': "Forest",
      'Fire Temple': "Fire",
      'Water Temple': "Water",
      'Shadow Temple': "Shadow",
      'Spirit Temple': "Spirit",
      'Bottom of the Well': "BOTW",
      'Gerudo Training Ground': "GTG",
      'Thieves Hideout': "Hideout",
      'Ganons Castle': "Ganons Castle"
    };
    let display = "Small Key Rings (";
    key_rings_list.forEach(element => {
      display += keyRingMap[element] + ", ";
    });
    display = display.substring(0, display.length - 2);
    display += ")";
    return display;
  }

  //Priorities:
  //Lowest: bridge condition and GBK requirement
  //MQ, Boss shortcuts
  //Open
  //Shuffled entrances
  //Shuffled items
  //Shuffled keys
  

  settings_paragraph_conversion_dictionary = {
    'bridge' : { 
      choices : {
        'open': 'Open Bridge',
        'vanilla': 'Vanilla Bridge Requirement',
        'stones' : '{bridge_stones} stones bridge',
        'medallions' : "{bridge_medallions} medallion bridge",
        'dungeons' : "{bridge_rewards} dungeon bridge",
        'tokens' : "{bridge_tokens} skull token bridge",
        'hearts' : "{bridge_hearts} heart bridge",
        'random' : 'Random Bridge Requirement'
      },
      priority : 0
    },
    'open_forest' : { 
      choices : {
      'open':        'Open Forest',
      'closed_deku': 'Closed Deku',
      'closed':      'Closed Forest'
      },
      priority : 2
    },
    'open_door_of_time' : { 
      choices : {
        true : 'Open DoT',
        false: 'Closed DoT'
      },
      priority : 2
    },
    'zora_fountain' : { 
      choices : {
        'closed' : undefined,
        'adult' : 'ZF Open for Adult',
        'open' : 'Open Fountain'
      },
      priority : 2
    },
    'gerudo_fortress' : { 
      choices : {
        'normal' : undefined,
        'fast': 'Rescue one carpenter',
        'open': 'Open Fortress'
      },
      priority : 2
    },
    'triforce_hunt': { 
      choices : {
        true : '{triforce_goal_per_world} piece Triforce Hunt',
        false : undefined
      },
      priority : 0
    },
    'bombchus_in_logic': { 
      choices : {
        true : 'Bombchus in logic',
        false : undefined
      },
      priority : 7
    },
    'one_item_per_dungeon' : { 
      choices : {
        true: "Dungeons have 1 major item",
        false: undefined
      },
      priority : 3
    },
    'plant_beans' : { 
      choices : {
        true : "Plant Magic Beans",
        false: undefined
      },
      priority : 6
    },
    'shuffle_ocarinas' : { 
      choices : {
        true: "Shuffle ocarinas",
        false: undefined
      },
      priority : 5
    },
    'shuffle_weird_egg' : { 
      choices : {
        true: "Shuffle weird egg",
        false: undefined
      },
      priority : 5
    },
    'shuffle_gerudo_card': { 
      choices : {
        true: "Shuffle Gerudo Card",
        false: undefined
      },
      priority : 5
    },
    'shuffle_song_items' : { 
      choices : {
        'song' : undefined,
        'dungeon' : "Songs on dungeon rewards",
        'any' : "Songsanity"
      },
      priority : 5
    },
    'shuffle_freestanding_items' : { 
      choices : {
        'off': undefined,
        'all' : "Shuffle freestanding items",
        'overworld': "Shuffle overworld freestanding items",
        'dungeons': "Shuffle dungeon freestanding items"
      },
      priority : 5
    },
    'shuffle_pots_crates' : { 
      choices : {
        'off': undefined,
        'all': "Shuffle pots/crates",
        'overworld': "Shuffle overworld pots/crates",
        'dungeons': "Shuffle dungeon pots/crates"
      },
      priority : 5
    },
    'shuffle_cows' : { 
      choices : {
        true : "Shuffle cows",
        false: undefined
      },
      priority : 5
    },
    'shuffle_beehives' : { 
      choices : {
        true: "Shuffle beehives",
        false: undefined
      },
      priority : 5
    },
    'shuffle_kokiri_sword': { 
      choices : {
        true: "Shuffle Kokiri Sword",
        false: undefined
      },
      priority : 5
    },
    'shuffle_beans' : { 
      choices : {
        true: "Shuffle magic beans",
        false: undefined
      },
      priority : 5
    },
    'shuffle_medigoron_carpet_salesman' : { 
      choices : {
        true: "Shuffle Medigoron/Carpet Salesman",
        false: undefined
      },
      priority : 5
    },
    'shuffle_frog_song_rupees' : { 
      choices : {
        true: "Shuffle frog song rupees",
        false: undefined
      },
      priority : 5
    },
    'shuffle_interior_entrances' : { 
      choices : {
        'off': undefined,
        'simple': "Shuffle interior entrances (simple)",
        'all': "Shuffle interior entrances"
      },
      priority: 4
    },
    'shuffle_grotto_entrances': { 
      choices : {
        true: "Shuffle grotto entrances",
        false: undefined
      },
      priority: 4
    },
    'shuffle_dungeon_entrances': { 
      choices : {
        true: "Shuffle dungeon entrances",
        false: undefined
      },
      priority : 4
    },
    'shuffle_bosses' : { 
      choices : {
        'off': undefined,
        'limited' : 'Age-restricted Boss Shuffle',
        'full' : 'Boss shuffle'
      },
      priority: 4
    },
    'shuffle_overworld_entrances' : { 
      choices : {
        true: "Shuffle overworld entrances",
        false: undefined
      },
      priority: 4
    },
    'owl_drops': { 
      choices : {
        true: "Random owl drops",
        false: undefined
      },
      priority: 4
    },
    'warp_songs' : { 
      choices : {
        true: "Random warp song destinations",
        false: undefined
      },
      priority: 4
    },
    'spawn_positions' : { 
      choices : {
        true: "Randomize overworld spawns",
        false: undefined
      },
      priority: 4
    },
    'mq_dungeons_mode': { 
      choices : {
        'vanilla': undefined,
        'mq': 'MQ Dungeons',
        'specific':"MQ Dungeons: {mq_dungeons_specific}",
        'count':"{mq_dungeons_count} MQ dungeons",
        'random':"Random # of MQ dungeons"
      },
      priority: 1
    },
    'shuffle_scrubs': { 
      choices : {
        'off': undefined,
        'low': 'Scrubsanity (Affordable)',
        'regular' : 'Scrubsanity (Expensive)',
        'random' : 'Scrubsanity (Random Prices)'
      },
      priority: 5
    },
    'shopsanity': { 
      choices : {
        'off': undefined,
        '1' : '1 item Shopsanity',
        '2' : '2 item Shopsanity',
        '3' : '3 item Shopsanity',
        '4' : 'Shopsanity'
      },
      priority: 5
    },
    'tokensanity': { 
      choices : {
        'off' : undefined,
        'dungeons' : "Shuffle dungeon skulls",
        'overworld' : "Shuffle overworld skulls",
        'all': "Skullsanity"
      },
      priority: 5
    },
    'shuffle_mapcompass': { 
      choices : {
        'remove' : "Remove maps/compasses",
        'startwith': "Start with maps/compasses",
        'vanilla': undefined,
        'dungeon': "Shuffle maps/compasses (Own Dungeon)",
        'overworld': "Shuffle maps/compasses (Overworld)",
        'any_dungeon': "Shuffle maps/compasses (Dungeons)",
        'keysanity': "Shuffle maps/compasses",
      },
      priority: 3
    },
    'shuffle_smallkeys' : { 
      choices : {
        'remove' : "Remove small keys",
        "vanilla" : undefined,
        "dungeon" : "Small keys in own dungeon",
        "overworld" : "Small keys in overworld",
        "any_dungeon" : "Small keys in any dungeon",
        "keysanity" : "Small Keysanity"
      },
      priority : 3
    },
    'shuffle_hideoutkeys' : { 
      choices : {
        'vanilla' : undefined,
        'overworld' : "Shuffle Hideout Keys (Overworld)",
        'any_dungeon' : "Shuffle Hideout Keys (Dungeons)",
        'keysanity': "Shuffle Hideout Keys"
      },
      priority: 5
    },
    'key_rings' : { 
      choices : 
        this.get_keyrings_string,
      priority: 3
    },
    'shuffle_bosskeys' : { 
      choices : {
        'remove' : "Remove boss keys",
        "vanilla" : undefined,
        "dungeon" : "Boss keys in own dungeon",
        "overworld" : "Boss keys in overworld",
        "any_dungeon" : "Boss keys in any dungeon",
        "keysanity" : "Boss Keysanity"
      },
      priority: 3
    },
    'shuffle_ganon_bosskey' : { 
      choices : {
        'remove' : "Remove GBK",
        'vanilla' : 'Vanilla GBK',
        'dungeon' : 'GBK in Ganons Castle',
        'overworld' : 'GBK in Overworld',
        'any_dungeon' : 'GBK in any dungeon',
        'keysanity' : 'Shuffle GBK anywhere',
        'on_lacs' : 'GBK on LACS',
        'stones' : '{ganon_bosskey_stones} stones GBK',
        'medallions': '{ganon_bosskey_medallions} medallions GBK',
        'dungeons': '{ganon_bosskey_rewards} dungeon GBK',
        'tokens': '{ganon_bosskey_tokens} skull GBK',
        'hearts': '{ganon_bosskey_hearts} heart GBK'
      },
      priority: 0
    },
    'dungeon_shortcuts_choice':{ 
      choices : {
        'off':undefined,
        'choice':"Dungeon Shortcuts: {dungeon_shortcuts}",
        'all': "Dungeon Shortcuts",
        'random': "Random # of Dungeon Shortcuts"
      },
      priority: 1
    },
    'trials_random': { 
      choices : {
        true: "Random # of Trials",
        false: undefined
      },
      priority: 1
    },
    'trials' : { 
      choices : {
        0: "Trials off",
        1: "1 Trial",
        2: "2 Trials",
        3: "3 Trials",
        4: "4 Trials",
        5: "5 Trials",
        6: "6 Trials"
      },
      priority: 1
    },
    'skip_child_zelda': { 
      choices : {
        true: "Skip Child Zelda",
        false: undefined
      },
      priority: 5
    },
    'free_scarecrow': { 
      choices : {
        true: "Free Scarecrow's Song",
        false: undefined
      },
      priority: 5
    },
    'correct_chest_appearances': { 
      choices : {
        'off': undefined,
        'textures': "Chest texture matches contents",
        'both': "Chest size/texture matches contents",
        'classic': "Chest size matches contents"
      },
      priority: 6
    },
    'correct_potcrate_appearances': { 
      choices : {
        'off' : undefined,
        'textures_content': "Pot/Crate textures match contents",
        'textures_unchecked': "Unchecked pot/crate textures"
      },
      priority: 6
    },
    'junk_ice_traps': { 
      choices : {
        'off' : undefined,
        'normal': undefined,
        'on' : "Extra ice traps",
        'mayhem': "Ice trap mayhem",
        'onslaught': "Ice trap onslaught"
      },
      priority: 6
    },
    'item_pool_value': { 
      choices : {
        'plentiful' : "Plentiful item pool",
        'balanced' : undefined,
        'scarce': "Scarce item pool",
        'minimal': "Minimal item pool"
      },
      priority: 7
    },
    'deadly_bonks': { 
      choices : {
        'ohko':      "One Bonk KO",
        'half':      'Bonks deal damage',
        'normal':    'Bonks deal damage',
        'double':    'Bonks deal damage',
        'quadruple': 'Bonks deal damage',
        'none': undefined
      },
      priority: 7
    },
    'no_collectible_hearts': { 
      choices : {
        true: "Hero mode",
        false: undefined
      },
      priority: 7
    },
    'blue_fire_arrows': { 
      choices : {
        true: "Blue fire arrows",
        false: undefined
      },
      priority: 8
    }
  };

  //Generate a paragraph of settings using the conversion dictionary. Order by priority
  convertSettingsToParagraphClient(settingsObj) {
    var paragraph = '';
    var paragraph_array = [];
    //Loop through every setting
    for(var setting_name in settingsObj){
      //Check if the setting is in our conversion dictionary
      if(setting_name in this.settings_paragraph_conversion_dictionary)
      {
        //extract the setting details
        var setting_value = settingsObj[setting_name].toString();
        var setting_options = this.settings_paragraph_conversion_dictionary[setting_name]['choices'];
        var priority = this.settings_paragraph_conversion_dictionary[setting_name]['priority'];
        //ignore GBK setting if triforce hunt
        if(setting_name  === 'shuffle_ganon_bosskey' && settingsObj['triforce_hunt'] == true)
        {
          continue;
        }
        if(paragraph_array[priority] == undefined)
        {
          paragraph_array[priority] = [];
        }
        var display_as = "";
        
        if(typeof(setting_options) === 'function')
        {
          display_as = setting_options(setting_value);
        }
        else
        {
          display_as = setting_options[setting_value];
        }
        if(display_as != undefined)
          {
            var start_index = display_as.indexOf("{");
            var end_index = display_as.indexOf("}");
            if(start_index >= 0 && end_index >= 0)
            {
              var sub_setting = display_as.substring(start_index + 1, end_index);
              var sub_setting_value = settingsObj[sub_setting];
              display_as = display_as.replace('{' + sub_setting + '}', sub_setting_value);
            }
            paragraph_array[priority].push(display_as);
          }
      }
        
    }
    //Build our paragraph
    for(const l1 of paragraph_array)
    {
      if(l1 != undefined)
      {
        for(const l2 of l1)
        {
          paragraph += l2 + ', ';
        }
      }
    }
    //Remove the final , 
    paragraph = paragraph.substring(0, paragraph.length - 2);
    return paragraph;
  }

  convertStringToSettings(settingsString: string) {
    var self = this;

    return new Promise(function (resolve, reject) {

      if (self.getGlobalVar('electronAvailable')) { //Electron

        post.send(window, 'convertStringToSettings', settingsString).then(event => {

          var listenerSuccess = post.once('convertStringToSettingsSuccess', function (event) {

            listenerError.cancel();

            let data = JSON.parse(event.data);
            resolve(data);
          });

          var listenerError = post.once('convertStringToSettingsError', function (event) {

            listenerSuccess.cancel();

            let data = event.data;

            console.error("[convertStringToSettings] Python Error:", data);
            reject(data);
          });

        }).catch(err => {
          console.error("[convertStringToSettings] Post-Robot Error:", err);
          reject(err);
        });
      }
      else { //Web
        let url = (<any>window).location.protocol + "//" + (<any>window).location.host + "/settings/get?version=" + self.getGlobalVar("webSourceVersion") + "&settingsString=" + settingsString;
        console.log("Request settings from:", url);

        self.http.get(url, { responseType: "json" }).toPromise().then(res => {
          resolve(res);
        }).catch(err => {
          console.error("[convertStringToSettings] Web Error:", err);
          reject(err);
        });
      }
    });
  }

  createPresetFileObject() {

    let presetsFile: any = {};

    //Create presets object
    Object.keys(this.generator_presets).forEach(presetKey => {
      let preset = this.generator_presets[presetKey];

      if (!("isNewPreset" in preset) && !("isDefaultPreset" in preset) && !("isProtectedPreset" in preset) && ("settings" in preset) && typeof (preset.settings) == "object" && Object.keys(preset.settings).length > 0) {
        //console.log("store " + presetKey, preset.settings);
        presetsFile[presetKey] = preset.settings;
      }
    });

    return presetsFile;
  }

  saveCurrentPresetsToFile() {

    if (this.getGlobalVar('electronAvailable')) { //Electron
      post.send(window, 'saveCurrentPresetsToFile', JSON.stringify(this.createPresetFileObject(), null, 4)).then(event => {
        console.log("presets saved to file");
      }).catch(err => {
        console.error(err);
      });
    }
    else { //Web
      if (this.getGlobalVar("appType") == "generator") { //Generator only
        try {
          localStorage.setItem("generatorPresets_" + this.getGlobalVar("webSourceVersion"), JSON.stringify(this.createPresetFileObject(), null, 4));
        } catch { }
      }
    }
  }

  generateSeedElectron(progressWindowRef: ProgressWindow, fromPatchFile: boolean = false, useStaticSeed: string = "") { //Electron only
    var self = this;

    return new Promise(function (resolve, reject) {

      let settingsMap = self.createSettingsFileObject(fromPatchFile, false, false, true);

      if (!settingsMap) {
        reject({ short: "Generation aborted.", long: "Generation aborted." });
        return;
      }

      //Hack: fromPatchFile forces generation count to 1 to avoid wrong percentage calculation
      if (fromPatchFile)
        settingsMap["count"] = 1;

      post.send(window, 'generateSeed', { settingsFile: settingsMap, staticSeed: useStaticSeed }).then(event => {

        var listenerProgress = post.on('generateSeedProgress', function (event) {

          let data = event.data;

          //console.log("progress report", data);

          if (progressWindowRef) {
            progressWindowRef.currentGenerationIndex = data.generationIndex;
            progressWindowRef.progressPercentageCurrent = data.progressCurrent;
            progressWindowRef.progressPercentageTotal = data.progressTotal;
            progressWindowRef.progressMessage = data.message;
            progressWindowRef.refreshLayout();
          }
        });

        var listenerSuccess = post.once('generateSeedSuccess', function (event) {

          listenerProgress.cancel();
          listenerCancel.cancel();
          listenerError.cancel();

          let data = event.data;
          resolve();
        });

        var listenerError = post.once('generateSeedError', function (event) {

          listenerProgress.cancel();
          listenerCancel.cancel();
          listenerSuccess.cancel();

          let data = event.data;

          console.error("[generateSeedElectron] Python Error:", data);
          reject(data);
        });

        var listenerCancel = post.once('generateSeedCancelled', function (event) {

          listenerProgress.cancel();
          listenerSuccess.cancel();
          listenerError.cancel();

          reject({ short: "Generation cancelled.", long: "Generation cancelled." });
        });
      }).catch(err => {
        console.error("[generateSeedElectron] Post-Robot Error:", err);
        reject({ short: err, long: err });
      });
    });
  }

  async cancelGenerateSeedElectron() { //Electron only

    if (!this.getGlobalVar('electronAvailable'))
      throw Error("electron_not_available");

    try {

      let event = await post.send(window, 'cancelGenerateSeed');
      let data = event.data;

      if (data == true)
        return;
      else
        throw Error("cancel_failed");
    }
    catch (err) {
      console.error("[cancelGenerateSeedElectron] Couldn't cancel due Post-Robot Error:", err);
      throw Error(err);
    }
  }

  hasRomExtension(fileName: string) {
    fileName = fileName.toLowerCase();
    return fileName.endsWith(".z64") || fileName.endsWith(".n64") || fileName.endsWith(".v64");
  }

  isValidFileObjectWeb(file: any) { //Web only
    return file && typeof (file) == "object" && file.name && file.name.length > 0;
  }

  readFileIntoMemoryWeb(fileObject: any, useArrayBuffer: boolean) { //Web only

    return new Promise<any>(function (resolve, reject) {

      let fileReader = new FileReader();
      fileReader.onabort = function (event) {
        console.error("Loading of the file was aborted unexpectedly!");
        reject();
      };
      fileReader.onerror = function (event) {
        console.error("An error occurred during the loading of the file!");
        reject();
      };
      fileReader.onload = function (event) {

        console.log("Read in file successfully");
        resolve(event.target["result"]); 
      };

      if (useArrayBuffer)
        fileReader.readAsArrayBuffer(fileObject);
      else
        fileReader.readAsText(fileObject);
    });
  }

  async readJsonFileIntoMemoryWeb(file: any, sizeLimit: number) { //Web only

    let fileJSON;

    if (this.hasRomExtension(file.name)) { //Not a ROM check...
      throw { error: "file_extension_was_rom" };
    }

    try {
      fileJSON = await this.readFileIntoMemoryWeb(file, false);
    }
    catch (ex) {
      throw { error: "file_read_error" };
    }

    if (!fileJSON || fileJSON.length < 1) {
      throw { error: "file_not_valid" };
    }

    if (fileJSON.length > sizeLimit) { //Impose size limit to avoid server overload
      throw { error: "file_too_big" };
    }

    //Test JSON parse it
    try {
      let jsonFileParsed = JSON.parse(fileJSON);

      if (!jsonFileParsed || Object.keys(jsonFileParsed).length < 1) {
        throw { error: "file_not_valid_json_empty" };
      }
    }
    catch (err) {
      console.error(err);
      throw { error: "file_not_valid_json_syntax", message: err.message };
    }

    return fileJSON;
  }

  async readPlandoFileIntoMemoryWeb(settingName: string, settingText: string) { //Web only

    let plandoFile = this.generator_settingsMap[settingName];

    if (!this.isValidFileObjectWeb(plandoFile))
      return null;

    //Try to resolve the plando file by reading it into memory
    console.log("Read Plando JSON file: " + plandoFile.name);

    try {
      let plandoFileText = await this.readJsonFileIntoMemoryWeb(plandoFile, 500000); //Will return the JSON file contents as text
      return plandoFileText;
    }
    catch (ex) {
      switch (ex.error) {       
        case "file_read_error": {
          throw { error: `An error occurred during the loading of the ${settingText}! Please try to enter it again.` };
        }
        case "file_not_valid": {
          throw { error: `The ${settingText} specified is not valid!` };
        }
        case "file_too_big": {
          throw { error: `The ${settingText} specified is too big! The maximum file size allowed is 500 KB.` };
        }
        case "file_not_valid_json_empty": {
          throw { error: `The ${settingText} specified is not valid JSON! Please verify the syntax.` };
        }
        case "file_not_valid_json_syntax": {
          throw { error: `The ${settingText} specified is not valid JSON! Please verify the syntax. Detail: ${ex.message}` };
        }
        default: {
          //Bubble error upwards
          throw ex;
        }
      }
    }
  }

  async generateSeedWeb(raceSeed: boolean = false, useStaticSeed: string = "") { //Web only

    //Plando Seed Logic
    let plandoFileSeed = null;
    if (this.generator_settingsMap["enable_distribution_file"]) {

      if (raceSeed) { //No support for race seeds
        throw { error: "Plandomizer for seed creation is currently not supported for race seeds due security concerns. Please use a normal seed instead!" };
      }

      let setting = this.findSettingByName("distribution_file");

      try {
        plandoFileSeed = await this.readPlandoFileIntoMemoryWeb("distribution_file", setting.text);
      }
      catch (ex) {
        switch (ex.error) {
          case "file_extension_was_rom": {
            throw { error_rom_in_plando: "Your Ocarina of Time ROM doesn't belong in a plandomizer setting. This entirely optional setting is used to plan out seeds before generation by manipulating spoiler log files. If you want to generate a normal seed instead, please click YES!", type: "distribution_file" };
          }
          default: {
            //Bubble error upwards
            throw ex;
          }
        }
      }
    }

    //Plando Cosmetics Logic
    let plandoFileCosmetics = null;
    if (this.generator_settingsMap["enable_cosmetic_file"]) {

      let setting = this.findSettingByName("cosmetic_file");

      try {
        plandoFileCosmetics = await this.readPlandoFileIntoMemoryWeb("cosmetic_file", setting.text);
      }
      catch (ex) {
        switch (ex.error) {
          case "file_extension_was_rom": {
            throw { error_rom_in_plando: "Your Ocarina of Time ROM doesn't belong in a plandomizer setting. This entirely optional setting is used to give you more control over your cosmetic and sound settings. If you want to generate a normal seed with regular cosmetics instead, please click YES!", type: "cosmetic_file" };
          }
          default: {
            //Bubble error upwards
            throw ex;
          }
        }
      }
    }

    let settingsFile = this.createSettingsFileObject(false, false, true, true);

    if (!settingsFile) {
      throw { error: "The generation was aborted due to previous errors!" };
    }

    //Add distribution file back into map as string if available, else clear it
    if (plandoFileSeed) {
      settingsFile["distribution_file"] = plandoFileSeed;
    }
    else {
      settingsFile["enable_distribution_file"] = false;
      settingsFile["distribution_file"] = "";
    }

    //Add cosmetics plando file back into map as string if available, else clear it
    if (plandoFileCosmetics) {
      settingsFile["cosmetic_file"] = plandoFileCosmetics;
    }
    else {
      settingsFile["enable_cosmetic_file"] = false;
      settingsFile["cosmetic_file"] = "";
    }

    if (raceSeed) {
      useStaticSeed = ""; //Static seeds aren't allowed in race mode
      settingsFile["create_spoiler"] = true; //Force spoiler mode to on
      settingsFile["encrypt"] = true;
    }
    else {
      delete settingsFile["encrypt"];
    }

    if (useStaticSeed) {
      console.log("Use Static Seed:", useStaticSeed);
      settingsFile["seed"] = useStaticSeed;
    }
    else {
      delete settingsFile["seed"];
    }

    //If spoiler log creation was intentionally disabled, warn user one time about the consequences
    try {
      let spoilerLogWarningSeen = localStorage.getItem("spoilerLogWarningSeen");

      if ((!spoilerLogWarningSeen || spoilerLogWarningSeen == "false") && settingsFile["create_spoiler"] == false) {
        localStorage.setItem("spoilerLogWarningSeen", JSON.stringify(true));
        throw { error_spoiler_log_disabled: "Generating a seed without a spoiler log means you won't be able to receive any help in case you get stuck! Would you rather generate a seed WITH a spoiler log?" };
      }
    }
    catch (err) { //Bubble through in case the warning should be displayed, ignore if local storage is not available
      if (err.hasOwnProperty('error_spoiler_log_disabled'))
        throw err;
    }

    console.log(settingsFile);
    console.log("Race Seed:", raceSeed);

    //Request Seed Generation
    let url = (<any>window).location.protocol + "//" + (<any>window).location.host + "/seed/create?version=" + this.getGlobalVar("webSourceVersion");
    console.log("Request seed id from:", url);

    try {
      let res = await this.http.post(url, JSON.stringify(settingsFile), { responseType: "text", headers: { "Content-Type": "application/json" } }).toPromise();
      return res;
    }
    catch (err) {
      console.error("[generateSeedWeb] Web Error:", err);
      throw err;
    }
  }

  async patchROMWeb() { //Web only

    //Plando Cosmetics Logic
    let plandoFileCosmetics = null;
    if (this.generator_settingsMap["enable_cosmetic_file"]) {

      try {
        plandoFileCosmetics = await this.readPlandoFileIntoMemoryWeb("cosmetic_file", "Cosmetic Plandomizer File");
      }
      catch (ex) {
        switch (ex.error) {
          case "file_extension_was_rom": {
            throw { error_rom_in_plando: "Your Ocarina of Time ROM doesn't belong in a plandomizer setting. This entirely optional setting is used to give you more control over your cosmetic and sound settings. If you want to patch your ROM with regular cosmetics instead, please click YES!", type: "cosmetic_file" };
          }
          default: {
            //Bubble error upwards
            throw ex;
          }
        }
      }
    }

    let settingsFile = this.createSettingsFileObject(true, false, false, true);

    if (!settingsFile) {
      throw { error: "The patching was aborted due to previous errors!" };
    }

    //Add cosmetics plando file back into map as string if available, else clear it
    if (plandoFileCosmetics) {
      settingsFile["cosmetic_file"] = plandoFileCosmetics;
    }
    else {
      settingsFile["enable_cosmetic_file"] = false;
      settingsFile["cosmetic_file"] = "";
    }

    if (typeof (<any>window).patchROM === "function") { //Try to call patchROM function on the DOM
      //Delay this so the async function can return early with "success"
      setTimeout(() => {
        (<any>window).patchROM(5, settingsFile); //Patch Version 5
      }, 0);
    }
    else {
      console.error("[patchROMWeb] Patcher not available!");
      throw { error: "Patcher not available!" };
    }
  }
}
