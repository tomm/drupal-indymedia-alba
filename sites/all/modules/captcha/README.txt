$Id: README.txt,v 1.15 2007/12/12 22:02:57 soxofaan Exp $

### CAPTCHA Readme

captcha.module is the basic CAPTCHA module that offers general CAPTCHA
administration and a simple math challenge.

text_captcha offers another simple text based challenge.

image_captcha offers an image based challenge.

Installation:
  Installation is like all normal modules (e.g. extract in the directory sites/all/modules)
  The basic CAPTCHA module has no dependencies, so nothing special is required.

Configuration:
  The configuration page is at admin/user/captcha, here you can configure
  the CAPTCHA module and enable challenges for the desired forms.

Using the Image CAPTCHA:
  If you plan on using image_captcha, you will have to install TTF fonts into
  the image_captcha/fonts or files directory.  You can find free fonts
  available at a number of different locations:
    - DejaVu:
      http://dejavu.sourceforge.net/wiki/index.php/Download
    - Gentium:
      http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&item_id=Gentium
    - Redhat:
      https://www.redhat.com/promo/fonts
    - Matt's Free Fonts Page:
      http://www.theory.org/~matt/strthrwr/fonts/free
