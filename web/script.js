document.addEventListener("DOMContentLoaded", function () {
  console.log("DOM Content Loaded");
  console.log("MonacoEditorLoader:", !!window.MonacoEditorLoader);
  console.log("Editor container:", document.getElementById("editor-container"));

  // Set status update callback
  if (window.ziaInterpreter && window.ziaInterpreter.setStatusCallback) {
    window.ziaInterpreter.setStatusCallback(function (message) {
      document.getElementById("status").textContent = message;
    });
  } else {
    console.error("ziaInterpreter not available");
    document.getElementById("status").textContent =
      "Erreur: ziaInterpreter non disponible";
  }

  // Try initializing the Monaco editor
  if (window.MonacoEditorLoader && window.MonacoEditorLoader.initEditor) {
    console.log("Initializing editor...");
    window.MonacoEditorLoader.initEditor("editor-container");
  } else {
    console.error("MonacoEditorLoader not available");
    document.getElementById("status").textContent =
      "Erreur: MonacoEditorLoader non disponible";
  }

  // Fallback to direct Monaco initialization
  setTimeout(function () {
    console.log("Attempting to initialize editor after delay");
    if (typeof require !== "undefined") {
      require(["vs/editor/editor.main"], function () {
        if (typeof monaco !== "undefined") {
          const container = document.getElementById("editor-container");
          if (container) {
            console.log("Creating editor directly");
          } else {
            console.error("Editor container not found");
          }
        } else {
          console.error("Monaco not defined");
        }
      });
    } else {
      console.error("Require not defined");
    }
  }, 1000);

  // Load the WebAssembly module
  if (window.ziaInterpreter && window.ziaInterpreter.loadModule) {
    window.ziaInterpreter
      .loadModule()
      .then(function (success) {
        if (success) {
          document.getElementById("status").textContent = "Prêt";
        } else {
          document.getElementById("status").textContent =
            "Erreur de chargement";
        }
      })
      .catch(function (error) {
        console.error("Error loading module:", error);
        document.getElementById("status").textContent =
          "Erreur de chargement: " + error.message;
      });
  }

  // Set up event listeners
  document.getElementById("run-button").addEventListener("click", function () {
    let sourceCode = "";
    if (window.MonacoEditorLoader && window.MonacoEditorLoader.getContent) {
      sourceCode = window.MonacoEditorLoader.getContent();
    } else if (window.editor) {
      sourceCode = window.editor.getValue();
    } else {
      // Fallback
      const editorElement = document.getElementById("editor-container");
      if (editorElement) {
        sourceCode = editorElement.textContent || "";
      }
    }

    const showBytecode = document.getElementById("bytecode-toggle").checked;

    try {
      // Show toast notification for execution
      showToast("Exécution en cours...", "info");

      // Update status
      document.getElementById("status").textContent = "Exécution...";
      document.querySelector(".status").classList.add("loading");

      // Clear output
      const outputElement = document.getElementById("output");
      if (outputElement) {
        outputElement.textContent = "";
      }

      if (window.MonacoEditorLoader && window.MonacoEditorLoader.setOutput) {
        window.MonacoEditorLoader.setOutput("");
      }

      // Run the code
      let result = "No interpreter available";
      if (window.ziaInterpreter) {
        if (showBytecode && window.ziaInterpreter.tokenize) {
          result = window.ziaInterpreter.tokenize(sourceCode);
        } else if (window.ziaInterpreter.interpret) {
          result = window.ziaInterpreter.interpret(sourceCode);
        }
      }

      // Display the result
      if (window.MonacoEditorLoader && window.MonacoEditorLoader.formatOutput) {
        window.MonacoEditorLoader.formatOutput(result);
      } else {
        const outputElement = document.getElementById("output");
        if (outputElement) {
          outputElement.textContent = result;
        }
      }

      // Update status
      document.getElementById("status").textContent = "Prêt";
      document.querySelector(".status").classList.remove("loading");

      // Show success toast
      showToast("Exécution terminée", "success");
    } catch (error) {
      console.error("Error running code:", error);
      const outputElement = document.getElementById("output");
      if (outputElement) {
        outputElement.textContent = "Erreur: " + error.message;
      }

      if (window.MonacoEditorLoader && window.MonacoEditorLoader.setOutput) {
        window.MonacoEditorLoader.setOutput("Erreur: " + error.message);
      }

      // Update status and show error toast
      document.getElementById("status").textContent = "Erreur";
      document.querySelector(".status").classList.remove("loading");
      document.querySelector(".status").classList.add("error");
      showToast("Erreur: " + error.message, "error");

      // Reset error status after 3 seconds
      setTimeout(() => {
        document.querySelector(".status").classList.remove("error");
        document.getElementById("status").textContent = "Prêt";
      }, 3000);
    }
  });

  // Dark mode toggle
  document
    .getElementById("darkmode-toggle")
    .addEventListener("change", function () {
      if (!this.checked) {
        document.body.classList.add("light-theme");
      } else {
        document.body.classList.remove("light-theme");
      }

      if (
        window.MonacoEditorLoader &&
        window.MonacoEditorLoader.toggleDarkMode
      ) {
        window.MonacoEditorLoader.toggleDarkMode(this.checked);
      }
    });

  // Clear output button
  document
    .getElementById("clear-output")
    .addEventListener("click", function () {
      const outputElement = document.getElementById("output");
      if (outputElement) {
        outputElement.textContent = "";
      }

      if (window.MonacoEditorLoader && window.MonacoEditorLoader.setOutput) {
        window.MonacoEditorLoader.setOutput("");
      }

      showToast("Sortie effacée", "info");
    });

  // Output type selector
  const outputTypeOptions = document.querySelectorAll(".output-type-option");
  outputTypeOptions.forEach((option) => {
    option.addEventListener("click", function () {
      outputTypeOptions.forEach((opt) => opt.classList.remove("active"));
      this.classList.add("active");

      // You can add the logic to filter output here
      const outputType = this.getAttribute("data-type");
      // For demonstration, we'll just show a toast
      showToast("Type de sortie: " + outputType, "info");
    });
  });

  // Resizer functionality
  const resizer = document.getElementById("resizer");
  const editorContainer = document.getElementById("editor-container");
  const outputContainer = document.querySelector(".output-container");

  let isResizing = false;

  resizer.addEventListener("mousedown", function (e) {
    isResizing = true;
    document.body.style.cursor = "col-resize";
  });

  document.addEventListener("mousemove", function (e) {
    if (!isResizing) return;

    const containerWidth = document.querySelector("main").offsetWidth;
    const percentage = (e.clientX / containerWidth) * 100;

    if (percentage > 20 && percentage < 80) {
      editorContainer.style.width = percentage + "%";
      outputContainer.style.width = 100 - percentage + "%";
    }
  });

  document.addEventListener("mouseup", function () {
    isResizing = false;
    document.body.style.cursor = "default";
  });

  // File tabs
  const fileTabs = document.querySelectorAll(".file-tab");
  fileTabs.forEach((tab) => {
    tab.addEventListener("click", function () {
      fileTabs.forEach((t) => t.classList.remove("active"));
      this.classList.add("active");

      // For demonstration, we'll just show a toast
      showToast("Fichier: " + this.textContent, "info");
    });
  });

  // New tab button
  document.getElementById("new-tab").addEventListener("click", function () {
    showToast("Nouveau fichier créé", "success");

    // In a real implementation, you would create a new tab here
    // and possibly open a dialog to name the file
  });

  // Settings button and panel
  const settingsButton = document.getElementById("settings-button");
  const settingsPanel = document.getElementById("settings-panel");
  const settingsClose = document.getElementById("settings-close");
  const settingsCancel = document.getElementById("settings-cancel");
  const settingsSave = document.getElementById("settings-save");

  settingsButton.addEventListener("click", function () {
    settingsPanel.classList.add("open");
    document.getElementById("key-shortcuts-panel").classList.remove("visible");
  });

  settingsClose.addEventListener("click", function () {
    settingsPanel.classList.remove("open");
  });

  settingsCancel.addEventListener("click", function () {
    settingsPanel.classList.remove("open");
  });

  settingsSave.addEventListener("click", function () {
    // For demonstration, we'll just show a toast
    showToast("Paramètres enregistrés", "success");
    settingsPanel.classList.remove("open");
  });

  // Key shortcuts panel
  let shortcutsVisible = false;
  settingsButton.addEventListener("mouseover", function () {
    document.getElementById("key-shortcuts-panel").classList.add("visible");
    shortcutsVisible = true;
  });

  document.addEventListener("click", function (e) {
    if (
      shortcutsVisible &&
      !settingsButton.contains(e.target) &&
      !document.getElementById("key-shortcuts-panel").contains(e.target)
    ) {
      document
        .getElementById("key-shortcuts-panel")
        .classList.remove("visible");
      shortcutsVisible = false;
    }
  });

  // Examples panel
  const showExamples = document.getElementById("show-examples");
  const examplesPanel = document.getElementById("examples-panel");
  const examplesClose = document.getElementById("examples-close");

  showExamples.addEventListener("click", function (e) {
    e.preventDefault();
    examplesPanel.classList.add("open");
  });

  examplesClose.addEventListener("click", function () {
    examplesPanel.classList.remove("open");
  });

  // Load examples
  const loadExampleButtons = document.querySelectorAll(".load-example");
  loadExampleButtons.forEach((button) => {
    button.addEventListener("click", function () {
      const exampleId = this.getAttribute("data-example");
      const exampleCode =
        this.closest(".example-item").querySelector(
          ".example-code"
        ).textContent;

      // Set the editor content
      if (window.MonacoEditorLoader && window.MonacoEditorLoader.setContent) {
        window.MonacoEditorLoader.setContent(exampleCode);
      } else if (window.editor) {
        window.editor.setValue(exampleCode);
      }

      // Close the examples panel
      examplesPanel.classList.remove("open");

      // Show notification
      showToast(
        "Exemple chargé: " +
          this.closest(".example-item").querySelector(".example-title span")
            .textContent,
        "success"
      );
    });
  });

  // Toast notification function
  function showToast(message, type = "info") {
    const toastContainer = document.getElementById("toast-container");

    const toast = document.createElement("div");
    toast.className = `toast ${type}`;

    let icon = "📋";
    if (type === "success") icon = "✅";
    if (type === "error") icon = "❌";
    if (type === "warning") icon = "⚠️";

    let title = "Information";
    if (type === "success") title = "Succès";
    if (type === "error") title = "Erreur";
    if (type === "warning") title = "Attention";

    toast.innerHTML = `
            <div class="toast-icon">${icon}</div>
            <div class="toast-content">
                <div class="toast-title">${title}</div>
                <div class="toast-message">${message}</div>
            </div>
        `;

    toastContainer.appendChild(toast);

    // Remove the toast after 3 seconds
    setTimeout(() => {
      toast.remove();
    }, 3000);
  }

  // Add keyboard shortcuts
  document.addEventListener("keydown", function (e) {
    // Ctrl+Enter to run code
    if (e.ctrlKey && e.key === "Enter") {
      document.getElementById("run-button").click();
    }

    // Ctrl+S to "save" (just show a toast for demonstration)
    if (e.ctrlKey && e.key === "s") {
      e.preventDefault(); // Prevent browser's save dialog
      showToast("Code sauvegardé", "success");
    }

    // F1 for help (just show examples panel for demonstration)
    if (e.key === "F1") {
      e.preventDefault();
      examplesPanel.classList.add("open");
    }
  });
});
