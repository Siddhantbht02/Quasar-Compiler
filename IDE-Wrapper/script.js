document.addEventListener('DOMContentLoaded', () => {
    // --- PROJECT FILES (sample placeholders) ---
    const projectFiles = {
      'C Source': {
        'scanner.l': `// scanner.l\n// sample scanner content\n`,
        'parser.y': `// parser.y\n// sample parser content\n`,
        'ast_printer.c': `// ast_printer.c\n// sample code\n`,
        'codegen.c': `// codegen.c\n// sample codegen\n`,
        'icg.c': `// icg.c\n// sample icg\n`,
        'main.c': `// main.c\nint main(){ return 0; }`,
        'semantic.c': `// semantic.c\n`,
        'symtab.c': `// symtab.c\n`
      },
      'Headers': {
        'ast.h': `// ast.h\n`,
        'ast_printer.h': `// ast_printer.h\n`,
        'codegen.h': `// codegen.h\n`,
        'icg.h': `// icg.h\n`,
        'semantic.h': `// semantic.h\n`,
        'symtab.h': `// symtab.h\n`
      },
      'Assembly': {
        'input.lang': `// Test Case: simple flow\nprint "Running Test...";\nconst user_age = 21;\nconst required_age = 18;\nlet access_code = 0;\nif (user_age >= required_age) {\n  print "Access Granted.";\n  access_code = 1;\n} else {\n  print "Access Denied.";\n  access_code = -1;\n}\nreturn access_code;`
      }
    };
  
    // --- DOM references ---
    const codeEditor = document.getElementById('code-editor');
    const editorTab = document.getElementById('editor-tab');
    const compileBtn = document.getElementById('compile-btn');
    const terminal = document.getElementById('terminal');
    const statusText = document.getElementById('status-text');
    const sidebar = document.getElementById('sidebar');
    const sidebarToggle = document.getElementById('sidebar-toggle');
    const resizer = document.getElementById('resizer');
    const footer = document.querySelector('footer');
  
    let activeFile = { category: 'Assembly', name: 'input.lang' };
    let isCompiling = false;
  
    // helper to pick icons
    const getFileIcon = (filename) => {
      if (/\.(c|y|l)$/i.test(filename)) return 'fas fa-code icon-c';
      if (/\.h$/i.test(filename)) return 'fas fa-heading icon-h';
      if (/\.lang$/i.test(filename)) return 'fas fa-file-code icon-lang';
      return 'fas fa-file-alt';
    };
  
    // populate files
    const populateFileExplorer = () => {
      const fileGroupElements = {
        'c-source': document.getElementById('c-source-files'),
        'headers': document.getElementById('header-files'),
        'assembly': document.getElementById('assembly-files')
      };
      const categoryMap = { 'C Source': 'c-source', 'Headers': 'headers', 'Assembly': 'assembly' };
  
      for (const categoryName in projectFiles) {
        const categoryId = categoryMap[categoryName];
        const fileList = fileGroupElements[categoryId];
        if (!fileList) continue;
        fileList.innerHTML = '';
        for (const filename in projectFiles[categoryName]) {
          const li = document.createElement('li');
          li.dataset.filename = filename;
          li.dataset.category = categoryName;
          li.innerHTML = `<i class="${getFileIcon(filename)}"></i><span class="filename-text">${filename}</span>`;
          li.addEventListener('click', () => {
            activeFile = { category: categoryName, name: filename };
            renderEditor();
          });
          fileList.appendChild(li);
        }
      }
    };
  
    // render editor
    const renderEditor = () => {
      editorTab.innerHTML = `<i class="${getFileIcon(activeFile.name)}"></i> ${activeFile.name}`;
      const content = projectFiles[activeFile.category] && projectFiles[activeFile.category][activeFile.name];
      codeEditor.value = content !== undefined ? content : '';
      codeEditor.disabled = !(activeFile.name === 'input.lang');
      // update active in sidebar
      document.querySelectorAll('.sidebar .file-list li').forEach(li => {
        const match = (li.dataset.filename === activeFile.name && li.dataset.category === activeFile.category);
        li.classList.toggle('active', match);
      });
    };
  
    // terminal typing helper
    const typeLine = (text, style = '', delay = 30) => {
      return new Promise(resolve => {
        setTimeout(() => {
          const line = document.createElement('div');
          line.className = `line ${style}`.trim();
          line.textContent = text;
          terminal.appendChild(line);
          terminal.scrollTop = terminal.scrollHeight;
          resolve();
        }, delay);
      });
    };
  
    // simple program simulator
    const simulateProgramExecution = (code) => {
      const output = [];
      try {
        const printRegex = /print\s+"([^"]+)";/g;
        const constRegex = /const\s+(\w+)\s*=\s*(-?\d+);/g;
        const letRegex = /let\s+(\w+)\s*=\s*(-?\d+);/g;
        const variables = {};
        let m;
        while ((m = constRegex.exec(code)) !== null) variables[m[1]] = parseInt(m[2], 10);
        while ((m = letRegex.exec(code)) !== null) variables[m[1]] = parseInt(m[2], 10);
        // pick first top-level print as heading
        const firstPrint = /print\s+"([^"]+)";/.exec(code);
        if (firstPrint) output.push(firstPrint[1]);
        const user_age = variables.user_age;
        const required_age = variables.required_age;
        if (typeof user_age !== 'undefined' && typeof required_age !== 'undefined') {
          if (user_age >= required_age) {
            const ifBlock = code.match(/if\s*\([\s\S]*?\)\s*\{([\s\S]*?)\}/);
            if (ifBlock && ifBlock[1]) {
              let r;
              while ((r = printRegex.exec(ifBlock[1])) !== null) output.push(r[1]);
            }
            variables.access_code = 1;
          } else {
            const elseBlock = code.match(/else\s*\{([\s\S]*?)\}/);
            if (elseBlock && elseBlock[1]) {
              let r;
              while ((r = printRegex.exec(elseBlock[1])) !== null) output.push(r[1]);
            }
            variables.access_code = -1;
          }
        } else {
          // fallback collect all prints
          let r;
          printRegex.lastIndex = 0;
          while ((r = printRegex.exec(code)) !== null) output.push(r[1]);
          variables.access_code = 0;
        }
        output.push(`Program returned: ${variables.access_code}`);
        return output;
      } catch (e) {
        return ["Error during simulation. Check input.lang syntax."];
      }
    };
  
    // compile/run simulation
    const handleCompile = async () => {
      if (isCompiling) return;
      isCompiling = true;
      compileBtn.disabled = true;
      if (statusText) statusText.textContent = 'Compiling...';
      // save current editor state
      if (!projectFiles[activeFile.category]) projectFiles[activeFile.category] = {};
      projectFiles[activeFile.category][activeFile.name] = codeEditor.value;
  
      terminal.innerHTML = '';
      await typeLine('bison -dy parser.y', 'command', 120);
      await typeLine('flex scanner.l', 'command', 100);
      await typeLine('gcc ast_printer.c codegen.c icg.c ... -o a.exe', 'command', 220);
      await typeLine('a.exe', 'command', 80);
      await typeLine('Parse successful! AST created.', '', 80);
      await typeLine('Assembly code generated in output.s', 'output-success', 60);
      await typeLine('gcc -o final_program.exe main.c output.s', 'command', 150);
      await typeLine('final_program.exe', 'command', 80);
  
      const programOutput = simulateProgramExecution(codeEditor.value);
      for (const line of programOutput) {
        await typeLine(line, 'program-output', 60);
      }
  
      if (statusText) statusText.textContent = 'Run finished successfully';
      isCompiling = false;
      compileBtn.disabled = false;
    };
  
    // --- UI wiring fixes ---
  
    // Sidebar toggle: store/restore prev width to avoid inline-style conflicts
    sidebarToggle.addEventListener('click', () => {
      const isCollapsed = sidebar.classList.contains('collapsed');
      if (!isCollapsed) {
        // collapse: save current effective width and then collapse
        const computed = getComputedStyle(sidebar).width;
        sidebar.dataset.prevWidth = sidebar.style.width || computed || '';
        // set explicit collapsed width so animation is smooth
        sidebar.style.width = `${parseInt(getComputedStyle(document.documentElement).getPropertyValue('--sidebar-collapsed')) || 64}px`;
        // small delay to allow inline width set then add class for consistent styling
        requestAnimationFrame(() => sidebar.classList.add('collapsed'));
        // also update aria
        sidebarToggle.setAttribute('aria-expanded', 'false');
      } else {
        // expand: remove collapsed class and restore width
        sidebar.classList.remove('collapsed');
        const prev = sidebar.dataset.prevWidth;
        if (prev) {
          // restore previous width (if it had been resized earlier)
          sidebar.style.width = prev;
        } else {
          // remove inline width and let CSS handle it
          sidebar.style.width = '';
        }
        sidebarToggle.setAttribute('aria-expanded', 'true');
      }
    });
  
    // Category toggles
    document.querySelectorAll('.category-header').forEach(header => {
      // set initial aria-expanded
      header.setAttribute('role', 'button');
      header.setAttribute('tabindex', '0');
      header.setAttribute('aria-expanded', 'true');
  
      const onToggle = () => {
        header.classList.toggle('collapsed');
        const listId = header.dataset.category + '-files';
        const list = document.getElementById(listId);
        if (!list) return;
        const collapsed = list.classList.toggle('collapsed');
        header.setAttribute('aria-expanded', String(!collapsed));
      };
  
      header.addEventListener('click', onToggle);
      header.addEventListener('keypress', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          onToggle();
        }
      });
    });
  
    // Sidebar resizer: update saved prevWidth on mouseup so toggle can restore it
    (() => {
      let dragging = false;
      let startX = 0;
      let startWidth = 0;
  
      resizer.addEventListener('mousedown', (e) => {
        dragging = true;
        startX = e.clientX;
        startWidth = sidebar.getBoundingClientRect().width;
        document.body.style.cursor = 'col-resize';
        document.body.style.userSelect = 'none';
      });
  
      document.addEventListener('mousemove', (e) => {
        if (!dragging) return;
        const dx = e.clientX - startX;
        const newWidth = startWidth + dx;
        const minW = 200;
        const maxW = 520;
        if (newWidth >= minW && newWidth <= maxW) {
          sidebar.style.width = `${newWidth}px`;
        }
      });
  
      document.addEventListener('mouseup', () => {
        if (!dragging) return;
        dragging = false;
        // save final width so toggle can restore it later
        sidebar.dataset.prevWidth = `${sidebar.getBoundingClientRect().width}px`;
        document.body.style.cursor = '';
        document.body.style.userSelect = '';
      });
  
      // touch support
      resizer.addEventListener('touchstart', (e) => {
        dragging = true;
        startX = e.touches[0].clientX;
        startWidth = sidebar.getBoundingClientRect().width;
        document.body.style.userSelect = 'none';
      });
      document.addEventListener('touchmove', (e) => {
        if (!dragging) return;
        const dx = e.touches[0].clientX - startX;
        const newWidth = startWidth + dx;
        const minW = 200;
        const maxW = 520;
        if (newWidth >= minW && newWidth <= maxW) {
          sidebar.style.width = `${newWidth}px`;
        }
      });
      document.addEventListener('touchend', () => {
        if (!dragging) return;
        dragging = false;
        sidebar.dataset.prevWidth = `${sidebar.getBoundingClientRect().width}px`;
        document.body.style.userSelect = '';
      });
    })();
  
    // Terminal resizer: create a visible grabber and implement vertical drag
    (() => {
      // Insert grabber between terminal-header and terminal-body
      const terminalHeader = document.querySelector('.terminal-header');
      if (!terminalHeader || !terminal) return;
  
      // create grabber element
      const grabber = document.createElement('div');
      grabber.className = 'terminal-grabber';
      // place after header
      terminalHeader.insertAdjacentElement('afterend', grabber);
  
      let dragging = false;
      let startY = 0;
      let startHeight = 0;
  
      const startDrag = (clientY) => {
        dragging = true;
        startY = clientY;
        // computed height of terminal
        startHeight = terminal.getBoundingClientRect().height;
        document.body.style.cursor = 'row-resize';
        document.body.style.userSelect = 'none';
      };
  
      const duringDrag = (clientY) => {
        if (!dragging) return;
        const dy = startY - clientY;
        // dragging up should increase terminal height (we subtract dy)
        const newHeight = startHeight + (-dy);
        const minH = parseInt(getComputedStyle(document.documentElement).getPropertyValue('--terminal-min')) || 100;
        const maxH = Math.round(window.innerHeight * 0.7);
        const clamped = Math.max(minH, Math.min(maxH, newHeight));
        terminal.style.height = clamped + 'px';
      };
  
      const stopDrag = () => {
        if (!dragging) return;
        dragging = false;
        document.body.style.cursor = '';
        document.body.style.userSelect = '';
        // ensure terminal stays within bounds
        const final = terminal.getBoundingClientRect().height;
        terminal.style.height = Math.max(parseInt(getComputedStyle(document.documentElement).getPropertyValue('--terminal-min')) || 100, Math.min(Math.round(window.innerHeight * 0.7), final)) + 'px';
      };
  
      grabber.addEventListener('mousedown', (e) => { e.preventDefault(); startDrag(e.clientY); });
      document.addEventListener('mousemove', (e) => { duringDrag(e.clientY); });
      document.addEventListener('mouseup', () => { stopDrag(); });
  
      // touch support
      grabber.addEventListener('touchstart', (e) => { startDrag(e.touches[0].clientY); });
      document.addEventListener('touchmove', (e) => { if (e.touches && e.touches[0]) duringDrag(e.touches[0].clientY); });
      document.addEventListener('touchend', () => { stopDrag(); });
    })();
  
    // compile wiring
    compileBtn.addEventListener('click', handleCompile);
  
    // keep projectFiles in sync on textarea input
    codeEditor.addEventListener('input', () => {
      if (activeFile && projectFiles[activeFile.category]) {
        projectFiles[activeFile.category][activeFile.name] = codeEditor.value;
      }
    });
  
    // initial population & render
    populateFileExplorer();
    renderEditor();
  
    // set initial aria state for sidebar button
    sidebarToggle.setAttribute('aria-expanded', sidebar.classList.contains('collapsed') ? 'false' : 'true');
  });
  