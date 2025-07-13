/**
 * load_emscripten.js
 * 
 * This file loads and initializes the WebAssembly module compiled with Emscripten.
 * Enhanced version with proper error handling to prevent abort() crashes.
 */

// Global variables to store the module and exported functions
let ziaModule = null;
let ziaInterpret = null;
let programExecuting = false;
let programOutput = '';

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
                const output = document.getElementById('output');
                if (output) {
                    // Store the output for later analysis
                    programOutput += text + '\n';
                    output.textContent += text + '\n';
                }
            },
            
            printErr: function(text) {
                // Only show actual compilation errors, not abort messages
                if (!text.includes('Aborted') && !text.includes('abort()')) {
                    const output = document.getElementById('output');
                    if (output) output.textContent += 'Error: ' + text + '\n';
                    console.error('Module Error:', text);
                }
            },
            
            setStatus: function(text) {
                updateStatus(text);
            },
            
            // Handle abort() calls gracefully - completely suppress for successful runs
            onAbort: function(what) {
                // If we're currently executing a program, check if it produced output
                if (programExecuting) {
                    // If there's output and no explicit error messages, this is likely normal termination
                    const hasValidOutput = programOutput.trim().length > 0;
                    const hasErrorMessages = programOutput.includes('Erreur') || 
                                           programOutput.includes('Error') || 
                                           programOutput.includes('ligne') ||
                                           programOutput.includes('manquant');
                    
                    if (hasValidOutput && !hasErrorMessages) {
                        // This is normal termination - just silently handle it
                        console.log('Program completed successfully');
                        updateStatus('Exécution terminée');
                        programExecuting = false;
                        return false;
                    }
                }
                
                // This might be an actual error
                console.error('WebAssembly module aborted:', what);
                updateStatus('Erreur de compilation');
                
                const output = document.getElementById('output');
                if (output && !programOutput.includes('===')) {
                    output.textContent += '\n=== Erreur de compilation ===\n';
                    output.textContent += 'Le code contient des erreurs qui empêchent la compilation.\n';
                }
                
                programExecuting = false;
                return false;
            },
            
            // Handle runtime errors
            onRuntimeError: function(error) {
                console.error('Runtime Error:', error);
                updateStatus('Erreur d\'exécution');
                
                const output = document.getElementById('output');
                if (output) {
                    output.textContent += '\nErreur d\'exécution: ' + error.message + '\n';
                }
                
                programExecuting = false;
                return false;
            },
            
            totalDependencies: 0,
            monitorRunDependencies: function(left) {
                this.totalDependencies = Math.max(this.totalDependencies, left);
                if (left) {
                    updateStatus('Chargement... (' + (this.totalDependencies - left) + '/' + this.totalDependencies + ')');
                } else {
                    updateStatus('Tous les téléchargements terminés');
                }
            },
            
            // Prevent the default abort behavior
            ABORT: false
        };
        
        // Add global error handlers to suppress abort-related errors
        const originalConsoleError = console.error;
        console.error = function(...args) {
            // Suppress abort-related console errors during program execution
            if (programExecuting && args.some(arg => 
                typeof arg === 'string' && (arg.includes('Aborted') || arg.includes('abort()')))) {
                return; // Silently ignore
            }
            originalConsoleError.apply(console, args);
        };
        
        window.addEventListener('error', function(event) {
            if (event.message && event.message.includes('Aborted') && programExecuting) {
                event.preventDefault();
                console.log('Suppressed abort error during program execution');
                return false;
            }
        });
        
        window.addEventListener('unhandledrejection', function(event) {
            if (event.reason && event.reason.toString().includes('Aborted') && programExecuting) {
                event.preventDefault();
                console.log('Suppressed abort rejection during program execution');
                return false;
            }
        });
        
        // Load the Emscripten-generated JavaScript file
        const script = document.createElement('script');
        script.src = 'zia.js';
        script.async = true;
        
        // Create a promise that resolves when the module is initialized
        const moduleLoaded = new Promise((resolve, reject) => {
            script.onload = () => {
                // Wait for the module to be initialized
                const checkInterval = setInterval(() => {
                    if (ziaModule || window.Module) {
                        clearInterval(checkInterval);
                        resolve();
                    }
                }, 100);
                
                // Timeout after 10 seconds
                setTimeout(() => {
                    if (!ziaModule && !window.Module) {
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
 * Safe wrapper for ccall that handles errors gracefully
 * @param {string} funcName - Name of the C function
 * @param {string} returnType - Return type
 * @param {Array} argTypes - Argument types
 * @param {Array} args - Arguments
 * @returns {*} - Function result or null on error
 */
function safeCCall(funcName, returnType, argTypes, args) {
    try {
        if (!window.Module || !window.Module.ccall) {
            throw new Error('Module not initialized');
        }
        
        return window.Module.ccall(funcName, returnType, argTypes, args);
    } catch (error) {
        // During program execution, suppress abort errors completely
        if (programExecuting && error.message && error.message.includes('Aborted')) {
            console.log('Program execution completed (abort suppressed)');
            updateStatus('Exécution terminée');
            return 0; // Return success code
        }
        
        console.error('Error calling', funcName, ':', error);
        
        // Handle other types of errors
        if (!error.message.includes('Aborted')) {
            const output = document.getElementById('output');
            if (output) {
                output.textContent += '\nErreur: ' + error.message + '\n';
            }
            updateStatus('Erreur lors de la compilation');
        }
        
        return null;
    }
}

/**
 * Interpret the given source code
 * @param {string} sourceCode - Source code to interpret
 * @param {boolean} activateBC - Activate bytecode
 * @param {boolean} activateTE - Activate type evaluation
 * @param {boolean} activateGC - Activate garbage collection
 * @returns {string} - Output of interpretation
 */
function interpret(sourceCode, activateBC, activateTE, activateGC) {
    // Clear previous output and reset state
    programOutput = '';
    programExecuting = true;
    
    const output = document.getElementById('output');
    if (output) {
        output.textContent = '========== Code En Compilation ==========\n\n';
    }
    
    const myTimeout = setTimeout(() => {
        compileCall({
            currentText: sourceCode, 
            activateBC: activateBC, 
            activateTE: activateTE, 
            activateGC: activateGC
        });
    }, 34);
}

function compileCall({currentText, activateBC, activateTE, activateGC}) {
    updateStatus('Compilation en cours...');
    
    const result = safeCCall(
        "runCompiler", // name of C function
        "int", // return type
        ["string", "bool", "bool", "bool"], // argument types
        [currentText, activateBC, activateTE, activateGC] // arguments
    );
    
    // Set a timer to finalize the execution status
    setTimeout(() => {
        if (programExecuting) {
            // Check if we have valid output
            if (programOutput.trim().length > 0 && 
                !programOutput.includes('Erreur') && 
                !programOutput.includes('Error')) {
                updateStatus('Exécution terminée');
            } else if (result !== null) {
                updateStatus('Compilation terminée');
            } else {
                updateStatus('Compilation échouée');
            }
            programExecuting = false;
        }
    }, 100);
    
    return result;
}

// Export the functions
window.ziaInterpreter = {
    loadModule: loadziaWasm,
    interpret: interpret,
    setStatusCallback: setStatusCallback
};