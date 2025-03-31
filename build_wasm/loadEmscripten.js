/**
 * load_emscripten.js
 * 
 * This file loads and initializes the WebAssembly module compiled with Emscripten.
 */

// Global variables to store the module and exported functions
let ziaModule = null;
let ziaTokenize = null;
let ziaInterpret = null;

// Status update callback
let onStatusChange = null;

/**
 * Set the status change callback
 * @param {Function} callback - Function to call with status updates
 */
function setStatusCallback(callback) {
    onStatusChange = callback;
}

/**
 * Update the status
 * @param {string} message - Status message
 */
function updateStatus(message) {
    if (onStatusChange) {
        onStatusChange(message);
    } else {
        console.log('Status: ' + message);
    }
}

/**
 * Load the WebAssembly module
 * @returns {Promise} - Resolves when the module is loaded
 */
async function loadziaWasm() {
    try {
        updateStatus('Chargement du module WebAssembly...');
        
        // Set up the Emscripten Module configuration
        window.Module = {
            preRun: [],
            postRun: [],
            print: function(text) {
                console.log('stdout: ' + text);
            },
            printErr: function(text) {
                console.error('stderr: ' + text);
            },
            onRuntimeInitialized: function() {
                ziaModule = Module;
                ziaTokenize = Module.cwrap('web_tokenize', 'string', ['string']);
                ziaInterpret = Module.cwrap('web_interpret', 'string', ['string']);
                
                updateStatus('Module WebAssembly initialisé');
            },
            setStatus: function(text) {
                updateStatus(text);
            },
            totalDependencies: 0,
            monitorRunDependencies: function(left) {
                this.totalDependencies = Math.max(this.totalDependencies, left);
                if (left) {
                    updateStatus('Chargement... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')');
                } else {
                    updateStatus('Tous les téléchargements terminés');
                }
            }
        };
        
        // Load the Emscripten-generated JavaScript file
        const script = document.createElement('script');
        script.src = 'zia.js';
        script.async = true;
        
        // Create a promise that resolves when the module is initialized
        const moduleLoaded = new Promise((resolve, reject) => {
            script.onload = () => {
                // Wait for the module to be initialized
                const checkInterval = setInterval(() => {
                    if (ziaModule && ziaTokenize && ziaInterpret) {
                        clearInterval(checkInterval);
                        resolve();
                    }
                }, 100);
                
                // Timeout after 10 seconds
                setTimeout(() => {
                    if (!ziaModule) {
                        clearInterval(checkInterval);
                        reject(new Error('Timeout initializing WebAssembly module'));
                    }
                }, 10000);
            };
            
            script.onerror = () => {
                reject(new Error('Failed to load zia.js'));
            };
        });
        
        // Add the script to the document
        document.body.appendChild(script);
        
        // Wait for the module to be loaded
        await moduleLoaded;
        return true;
        
    } catch (error) {
        updateStatus('Erreur lors du chargement du module: ' + error.message);
        console.error('Error loading zia module:', error);
        return false;
    }
}

/**
 * Tokenize the given source code
 * @param {string} sourceCode - Source code to tokenize
 * @returns {string} - JSON string of tokens
 */
function tokenize(sourceCode) {
    if (!ziaTokenize) {
        throw new Error('Module not initialized. Call loadziaWasm() first.');
    }
    
    try {
        return ziaTokenize(sourceCode);
    } catch (error) {
        console.error('Error during tokenization:', error);
        return JSON.stringify({
            type: 'ERREUR',
            message: error.message,
            line: 0
        });
    }
}

/**
 * Interpret the given source code
 * @param {string} sourceCode - Source code to interpret
 * @returns {string} - Output of interpretation
 */
function interpret(sourceCode) {
    if (!ziaInterpret) {
        throw new Error('Module not initialized. Call loadziaWasm() first.');
    }
    
    try {
        return ziaInterpret(sourceCode);
    } catch (error) {
        console.error('Error during interpretation:', error);
        return JSON.stringify({
            type: 'ERREUR',
            message: error.message,
            line: 0
        });
    }
}

// Export the functions
window.ziaInterpreter = {
    loadModule: loadziaWasm,
    tokenize: tokenize,
    interpret: interpret,
    setStatusCallback: setStatusCallback
};