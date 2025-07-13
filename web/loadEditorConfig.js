/**
 * load_monaco_config.js
 * 
 * This file contains configuration for the Monaco editor.
 */

// Monaco editor loader configuration
require.config({
    paths: {
        'vs': '/build_wasm/node_modules/monaco-editor/min/vs'
    }
});

// Monaco WorkerManager configuration
self.MonacoEnvironment = {
    getWorkerUrl: function(moduleId, label) {
        //return `/monaco/vs/${label}/${label}.worker.js`;
        if (label === 'json') {
            return '/build_wasm/node_modules/monaco-editor/min/vs/language/json/json.worker.js';
        }
        if (label === 'css' || label === 'scss' || label === 'less') {
            return '/build_wasm/node_modules/monaco-editor/min/vs/language/css/css.worker.js';
        }
        if (label === 'html' || label === 'handlebars' || label === 'razor') {
            return '/build_wasm/node_modules/monaco-editor/min/vs/language/html/html.worker.js';
        }
        if (label === 'typescript' || label === 'javascript') {
            return '/build_wasm/node_modules/monaco-editor/min/vs/language/typescript/ts.worker.js';
        }
        return '/build_wasm/node_modules/monaco-editor/min/vs/editor/editor.worker.js';
    }
};