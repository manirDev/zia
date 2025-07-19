# Installation de Zia
## Configurez votre environnement de développement Zia

Cette section vous guide pas à pas dans l'installation et la configuration de Zia sur votre système. Que vous préfériez une installation locale complète ou une utilisation en ligne, vous trouverez ici toutes les informations nécessaires pour commencer à programmer en français.

---

## 🌐 Option 1 : Utilisation en ligne (Recommandée pour débuter)

### **Interface web interactive**
La méthode la plus simple pour découvrir Zia est d'utiliser l'éditeur en ligne, parfait pour l'apprentissage et les premiers tests.

**Accès immédiat** :

- 🔗 Rendez-vous sur [l'éditeur Zia en ligne](https://manirdev.github.io/zia/build_wasm/)
- ✨ Aucune installation requise
- 💻 Fonctionne directement dans votre navigateur
- 🎯 Éditeur intégré avec coloration syntaxique de Zia

**Avantages** :

- ⚡ Démarrage instantané
- 🔄 Toujours à jour avec la dernière version
- 🌍 Accessible depuis n'importe où

**Limitations** :

- 🌐 Nécessite une connexion internet
- 💾 Pas de sauvegarde locale automatique

---

## 💻 Option 2 : Installation locale

### **Prérequis système**

Avant d'installer Zia, assurez-vous que votre système dispose des éléments suivants :

**Outils nécessaires** :

- 🔧 Git (pour cloner le dépôt)
- 🏗️ Compilateur C (GCC ou Clang)
- 📦 Make ou système de build compatible

### **Installation depuis GitHub**

#### **Méthode recommandée - Clonage du dépôt**
```bash
# Clonage du dépôt officiel Zia
git clone https://github.com/manirDev/zia.git

# Accès au répertoire du projet
cd zia

# Exploration de la structure du projet
ls -la

# Vérification des instructions de compilation
cat README.md
```

## ⚙️ Configuration post-installation

### **Compilation et vérification**

Après avoir cloné le dépôt, compilez et testez Zia :

```bash
# Affichage de l'aide du Makefile
make help

# Construction de l'exécutable natif
make build

# Test en mode interactif (REPL)
make run

# Création d'un fichier de test simple
echo 'var nom = "Monde";' > start.zia
echo 'afficher "Bonjour ", nom, "!";' >> start.zia

# Exécution du programme de test avec make
make start

# Ou exécution directe avec l'exécutable généré
./zia.out start.zia
```

**Résultat attendu** :
```
Bonjour Monde !
```

**Aide complète du Makefile** :
```bash
make help
```
```
ZIA Interpreter Makefile
-------------------------
Available targets:
  build:    Build native executable
  run:      Run the interpreter in interactive mode
  start:    Run the interpreter with start.zia file
  web:      Build the WebAssembly version
  deps:     Install dependencies (Monaco Editor)
  websetup: Complete setup for web version
  serve:    Start a development web server
  clean:    Remove build artifacts
  help:     Show this help message
```

## 🔧 Compilation et configuration

### **Commandes de compilation principales**

```bash
# Affichage de l'aide complète
make help

# Construction de base de l'interpréteur natif
make build

# Nettoyage des fichiers de compilation
make clean

# Reconstruction complète
make clean build

# Test en mode interactif
make run

# Exécution avec le fichier start.zia
make start
```


### **Support et communauté**

- 🐛 **Bugs** : Signalez les problèmes sur [GitHub Issues](https://github.com/manirDev/zia/issues)

---

## ✅ Prochaines étapes

Maintenant que Zia est installé, vous êtes prêt à :

1. **[Découvrir la syntaxe](syntax.md)** - Apprenez les bases du langage
2. **[Écrire votre premier programme](first-step.md)** - Créez votre première application
3. **[Explorer les exemples](https://github.com/manirDev/zia/tree/master/tests/exercices)** - Inspirez-vous des projets existants

---

*💡 **Astuce** : Bookmark l'éditeur en ligne pour vos tests rapides et utilisez l'installation locale pour vos projets sérieux.*

*🔄 **Mises à jour** : Restez informé des nouvelles versions en suivant le [dépôt GitHub](https://github.com/manirDev/zia) et en activant les notifications.*