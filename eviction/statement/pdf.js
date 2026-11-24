const {chromium}=require('playwright');
(async()=>{const d=process.argv[2];const b=await chromium.launch();const p=await b.newPage();
for(const L of ['en','fa']){await p.goto(`file://${d}/statement-${L}.html`);await p.evaluate(()=>document.fonts.ready);
 await p.pdf({path:`${d}/statement-${L}.pdf`,format:'A4',margin:{top:'15mm',bottom:'15mm',left:'15mm',right:'15mm'},printBackground:true});}
await b.close();})();
