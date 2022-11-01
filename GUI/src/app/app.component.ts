import { Component, ViewContainerRef, ElementRef, ViewEncapsulation, Renderer2,  Inject } from '@angular/core';
import { PipeTransform, Pipe } from '@angular/core';
import { Router } from '@angular/router';
import { DomSanitizer } from "@angular/platform-browser";

import { GUIGlobal } from './providers/GUIGlobal';
import { DOCUMENT } from "@angular/common";

@Pipe({ name: 'bypassSecurity' })
export class BypassSecurityPipe implements PipeTransform {
  constructor(private sanitizer: DomSanitizer) { }

  transform(url) {
    return this.sanitizer.bypassSecurityTrustResourceUrl(url);
  }
}

@Component({
  selector: 'ngx-app',
  template: `<body id="generator" class="nb-theme-default">
    <router-outlet></router-outlet>
  </body>`,
  // Shadow-DOM requires code tweaks within Nebular.
  // Style interference with website is addressed by usage of selector specificity.
  encapsulation: ViewEncapsulation.None
})
export class AppComponent {
  private changes: MutationObserver;

  constructor(public viewContainerRef: ViewContainerRef,
              private elm: ElementRef,
              private router: Router,
              private global: GUIGlobal,
              private readonly renderer: Renderer2,
              @Inject(DOCUMENT) private readonly document: Document,
  ) {
    global.globalInit(elm.nativeElement.id);
    this.observeInitialization();

    //Route manually at the start to avoid URL changes in the browser
    this.router.navigate(['/pages/generator'], { skipLocationChange: true });
  }

  private observeInitialization(): void {
    const outerBody = this.document.getElementsByTagName('body')[0];

    this.changes = new MutationObserver((mutations: MutationRecord[]) => {
      mutations.forEach((mutation: MutationRecord) => {
        if (this.isInitializationDone(mutation, outerBody)) {
          this.fixNebularThemingScope();
        }
      });
    });

    this.changes.observe(outerBody, {
      attributeFilter: ['class'],
    });
  }

  private fixNebularThemingScope() {
    const generatorBody = this.document.querySelector('body#generator');
    const outerBody = this.document.getElementsByTagName('body')[0];

    if (outerBody && outerBody !== generatorBody && outerBody.classList.contains('nb-theme-default')) {
      this.renderer.removeClass(outerBody, `nb-theme-default`);

      if (generatorBody && !generatorBody.classList.contains('nb-theme-default')) {
        this.renderer.addClass(generatorBody, `nb-theme-default`);
      }
    }
  }

  private isInitializationDone(mutation: MutationRecord, outerBody: HTMLBodyElement) {
    return mutation.attributeName === 'class' && outerBody.classList.contains('pace-done');
  }
}
