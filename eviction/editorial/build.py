#!/usr/bin/env python3
"""Builds editorial.html and editorial.pdf (needs node + playwright/chromium)."""
import os, re, html, subprocess
D = os.path.dirname(os.path.abspath(__file__))
subprocess.run(["python3", os.path.join(D, "figs.py")], check=True)
body = open(os.path.join(D, "content.html")).read()
for f in os.listdir(D):
    if f.startswith("fig_") and f.endswith(".svg"):
        body = body.replace("{{" + f[:-4] + "}}", open(os.path.join(D, f)).read())
code = html.escape(open(os.path.join(D, "solution_editorial.cpp")).read())
code = re.sub(r"(//[^\n]*)", r'<span class="cm">\1</span>', code)
body = body.replace("{{code}}", code)
assert "{{" not in body, re.findall(r"\{\{\w+\}\}", body)
page = f'<!doctype html><html lang="fa" dir="rtl"><head><meta charset="utf-8"><title>Express Eviction Editorial</title><link rel="stylesheet" href="style.css"></head><body>{body}</body></html>'
open(os.path.join(D, "editorial.html"), "w").write(page)
js = """const {chromium}=require('playwright');(async()=>{const b=await chromium.launch();const p=await b.newPage();
await p.goto('file://'+process.argv[1]+'/editorial.html');await p.evaluate(()=>document.fonts.ready);
await p.pdf({path:process.argv[1]+'/editorial.pdf',preferCSSPageSize:true,printBackground:true,displayHeaderFooter:true,
headerTemplate:'<span></span>',footerTemplate:'<div style="font-size:8px;width:100%;text-align:center;color:#999"><span class=pageNumber></span> / <span class=totalPages></span></div>'});
await b.close();})();"""
subprocess.run(["node", "-e", js, D], check=True)
print("built editorial.pdf")
