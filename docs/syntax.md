# Syntaxe
## Les règles grammaticales de Zia

> *"La syntaxe d'un langage de programmation est comme la grammaire d'une langue vivante : elle donne structure et sens à nos pensées créatrices."*

---

## 🎭 L'élégance française dans le code

Imaginez pouvoir écrire du code avec la même fluidité que vous rédigez une lettre en français. C'est exactement ce que propose Zia : une syntaxe qui respecte non seulement les règles techniques de la programmation, mais aussi l'esprit et la logique de la langue française.

Zia hérite de la robustesse structurelle du langage C tout en substituant ses mots-clés anglais par leurs équivalents français naturels. Cette approche crée une harmonie unique entre la rigueur algorithmique et l'élégance linguistique française.

---

## 🗝️ Le vocabulaire fondamental de Zia

### **Les mots-clés : votre palette d'expression**

Chaque mot-clé de Zia a été soigneusement sélectionné pour respecter l'usage français tout en conservant la précision technique nécessaire. Voici votre vocabulaire de base :

| **Mot-clé Zia** | **Rôle et nuance** | **Équivalent technique** |
|-----------------|-------------------|-------------------------|
| `afficher` | *Révéler, présenter au monde* | print/printf |
| `var` | *Déclarer l'existence d'une donnée* | var/let |
| `fonction` | *Créer un bloc d'actions réutilisable* | function |
| `retourner` | *Renvoyer un résultat vers l'appelant* | return |
| `si` | *Introduire une condition* | if |
| `sinon si` | *Condition alternative* | else if |
| `sinon` | *Action alternative* | else |
| `pour` | *Itération déterminée* | for |
| `tantque` | *Répétition conditionnelle* | while |
| `selon` | *Sélection parmi plusieurs cas* | switch |
| `cas` | *Option spécifique dans une sélection* | case |
| `quitter` | *Sortir d'une boucle ou d'un bloc* | break |
| `continuer` | *Passer à l'itération suivante* | continue |
| `et` | *Conjonction logique inclusive* | and/&& |
| `ou` | *Disjonction logique* | or/\|\| |
| `vrai` | *Valeur de vérité positive* | true |
| `faux` | *Valeur de vérité négative* | false |
| `nul` | *Absence de valeur* | null |
| `classe` | *Modèle d'objet* | class |
| `ceci` | *Référence à l'instance courante* | this |
| `super` | *Référence à la classe parente* | super |

### **La poésie des mots choisis**

Chaque terme français de Zia porte en lui une richesse sémantique qui dépasse la simple traduction :

- **`afficher`** évoque l'idée de montrer fièrement, comme on affiche ses couleurs
- **`retourner`** suggère un voyage de la donnée qui revient vers son origine
- **`tantque`** exprime la persistence dans le temps, la continuation d'un état
- **`sinon`** apporte cette nuance française de l'alternative courtoise

---

## 🏗️ Architecture d'un programme Zia

### **Structure générale : la logique française**

Un programme Zia suit une architecture claire qui respecte l'ordre naturel de la pensée française :

```zia
// Déclarations (ce que nous possédons)
var message = "Bonjour Zia !";
var compteur = 0;

// Actions (ce que nous faisons)
fonction saluer() {
    afficher message;
    compteur = compteur + 1;
}

// Exécution (ce que nous accomplissons)
saluer();
```

### **Les commentaires : vos notes personnelles**

Zia supporte naturellement les commentaires français avec les accents :

```zia
// Ceci est un commentaire sur une ligne
/* Ceci est un commentaire
   sur plusieurs lignes
   parfait pour les explications détaillées */

var âge = 25; // Les accents sont parfaitement supportés
```

---

## 📦 Variables : donner vie aux données

### **Déclaration : l'acte de naissance des données**

En Zia, créer une variable, c'est donner naissance à un concept dans l'univers de votre programme :

```zia
var nom = "Marie";           // Une identité textuelle
var âge = 28;                // Un nombre entier
var taille = 1.65;           // Un nombre décimal
var estÉtudiante = vrai;     // Une vérité logique
var hobby = nul;             // Une valeur encore indéterminée
```

### **Conventions de nommage à la française**

Zia encourage l'utilisation de noms expressifs en français :

```zia
// ✅ Style français recommandé
var nombreDEtudiants = 42;
var moyenneGénérale = 15.8;
var estConnecté = faux;

// ✅ Accents supportés naturellement
var prénom = "François";
var numéroTéléphone = "0123456789";
var estMajeur = vrai;
```

### **Types de données : la richesse de l'expression**

```zia
// Nombres entiers
var population = 67000000;
var température = -5;

// Nombres décimaux
var pi = 3.14159;
var pourcentage = 87.5;

// Textes (chaînes de caractères)
var citation = "La programmation est un art";
var email = "contact@exemple.fr";

// Valeurs logiques
var estValide = vrai;
var estTerminé = faux;

// Valeur nulle
var résultat = nul; // En attente d'une valeur
```

---

## 🎯 Structures de contrôle : la logique en action

### **Conditions : l'art de décider**

La structure conditionnelle en Zia reflète la logique française naturelle :

```zia
var note = 16;

si (note >= 10) {
    afficher "Félicitations, vous êtes reçu !";
} sinon {
    afficher "Courage, vous pouvez réessayer.";
}
```

### **Conditions complexes : nuancer la pensée**

```zia
var âge = 25;
var aLepermis = vrai;

si (âge >= 18 et aLepermis) {
    afficher "Vous pouvez conduire !";
} sinon si (âge >= 18) {
    afficher "Pensez à passer votre permis.";
} sinon {
    afficher "Vous êtes encore mineur.";
}
```

### **Boucles : la répétition maîtrisée**

**La boucle `pour` - itération déterminée :**

```zia
afficher "Compte à rebours :";
pour (var i = 10; i > 0; i = i - 1) {
    afficher i;
}
afficher "Décollage !";
```

**La boucle `tantque` - persévérance conditionnelle :**

```zia
var essais = 0;
var réussi = faux;

tantque (essais < 3 et !réussi) {
    afficher "Tentative numéro : " + (essais + 1);
    // Simulation d'un test
    réussi = (essais == 2); // Réussit à la 3ème tentative
    essais = essais + 1;
}
```

---

## 🔧 Fonctions : la modularité élégante

### **Définition : créer ses propres actions**

```zia
fonction direBonjour(prénom) {
    afficher "Bonjour " + prénom + " !";
    afficher "Comment allez-vous ?";
}

fonction calculerAire(longueur, largeur) {
    var aire = longueur * largeur;
    retourner aire;
}
```

### **Utilisation : donner vie aux fonctions**

```zia
// Appels simples
direBonjour("Sophie");
direBonjour("Thomas");

// Utilisation avec valeur de retour
var surfaceSalon = calculerAire(5.2, 4.8);
afficher "La surface du salon est : " + surfaceSalon + " m²";
```

---

## 🎨 Exemples pratiques : l'art en action

### **Programme calculatrice simple**

```zia
fonction calculatrice() {
    var nombre1 = 15;
    var nombre2 = 7;
    
    afficher "=== Calculatrice Zia ===";
    afficher "Premier nombre : " + nombre1;
    afficher "Second nombre : " + nombre2;
    
    afficher "Addition : " + (nombre1 + nombre2);
    afficher "Soustraction : " + (nombre1 - nombre2);
    afficher "Multiplication : " + (nombre1 * nombre2);
    afficher "Division : " + (nombre1 / nombre2);
}

calculatrice();
```

### **Générateur de compliments**

```zia
fonction complimenter(nom, adjectif) {
    var compliments = [
        "Vous êtes formidable",
        "Votre travail est excellent", 
        "Continuez ainsi"
    ];
    
    pour (var i = 0; i < 3; i = i + 1) {
        afficher nom + ", " + compliments[i] + " !";
    }
}

complimenter("Marie", "brillante");
```

---

## 🌟 Spécificités françaises de Zia

### **Support naturel des accents**

```zia
var créativité = "infinie";
var rêve = "réalisable";
var qualité = "française";

fonction vérifierMaîtrise(niveau) {
    si (niveau >= 80) {
        retourner "Maîtrise excellente !";
    } sinon {
        retourner "Continuez à progresser.";
    }
}
```

### **Logique de lecture naturelle**

Zia privilégie la lecture fluide, presque littéraire :

```zia
var utilisateur = "Pierre";
var estConnecté = vrai;

si (utilisateur != nul et estConnecté) {
    afficher "Bienvenue " + utilisateur + " !";
    afficher "Vous êtes maintenant connecté.";
} sinon {
    afficher "Veuillez vous connecter.";
}
```

---

## 🎓 Règles de style recommandées

### **Indentation : la respiration du code**

```zia
fonction exempleIndentation() {
    var condition = vrai;
    
    si (condition) {
        afficher "Première ligne indentée";
        
        si (condition) {
            afficher "Seconde niveau d'indentation";
        }
    }
}
```

### **Nommage : la clarté française**

```zia
// ✅ Noms expressifs et français
var compteurVisiteurs = 0;
var listeÉtudiants = [];
var estAuthentifié = faux;

fonction calculerMoyenneClasse(notes) {
    // Code clair et bien nommé
}

// ❌ Éviter les abréviations obscures
var cptVst = 0;        // Pas assez clair
var lstEtd = [];       // Difficile à comprendre
```

---

## 🚀 Vers la maîtrise syntaxique

La syntaxe de Zia n'est pas qu'un ensemble de règles techniques : c'est un langage d'expression qui vous permet de traduire vos idées en instructions compréhensibles par la machine, tout en conservant l'élégance et la précision de la langue française.

Chaque mot-clé, chaque structure, chaque convention a été pensée pour créer une harmonie entre votre pensée naturelle en français et les exigences de la programmation moderne. En maîtrisant cette syntaxe, vous acquérez non seulement les bases techniques nécessaires, mais aussi une nouvelle façon de structurer et d'exprimer vos idées créatrices.

Dans le chapitre suivant, nous explorerons en détail les variables et les types de données, approfondissant ainsi votre compréhension des fondements de Zia.

---

*La syntaxe est votre vocabulaire, la logique sera votre grammaire, et la créativité, votre style littéraire dans l'art de programmer en français.*