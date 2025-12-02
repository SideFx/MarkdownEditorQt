### MarkdownEditor (Qt) - (w) 2025 Jan Buchholz 

MarkdownEditorQt is a Qt6-based application for editing and previewing Markdown documents.
- Built on the md4c and md4c-html libraries for fast and accurate HTML generation
- Supports embedded images in PNG, JPEG, WebP, and SVG formats
- Fully aligned with the CommonMark standard, with highlighting emulated via HTML tags
- Includes a live preview feature (see screenshot below)


![Screenshot](images/img01.png)



#### Credits
[Special thanks to Martin Mitáš (mity) and all contributors to the md4c library.](https://github.com/mity/md4c)


#### Updates

**2025-11-29:**
- Fixed text alignment in viewer (no more occasional line breaks)
- Performed a quick code review

**2025-11-30:**
- Fixed Qt save dialog not appending the .md extension
- Enabled **strikethrough** and **tables** in md4c
- Hacked md4c-html.c: Replaced &lt;del&gt; tags with &lt;s&gt; tags, so that QTextBrowser finally understands ~strikethrough~.

**2025-12-02:**
- Fixed dirty flag


