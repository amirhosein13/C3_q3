#!/bin/bash
# Builds statement-en/fa.html and .pdf (needs pandoc + node playwright/chromium)
set -e; cd "$(dirname "$0")"
for L in en fa; do src=index.md; dir=ltr; [ $L = fa ] && src=index-fa.md && dir=rtl
  pandoc $src -s --mathml --metadata pagetitle="Express Eviction" -V lang=$L -V dir=$dir -c style.css -o statement-$L.html; done
node pdf.js "$PWD"
