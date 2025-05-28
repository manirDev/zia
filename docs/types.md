# Variables et Types
## Gestion des données en français

> *"Une variable est comme un tiroir étiquetté dans l'esprit de votre programme : elle garde précieusement vos données en attendant que vous en ayez besoin."*

---

## 🎭 L'art de nommer et typer à la française

Dans l'univers de Zia, chaque donnée est un personnage de votre histoire algorithmique. Les variables sont les noms que vous donnez à ces personnages, et les types définissent leur nature profonde. Cette approche française du typage dynamique vous libère des contraintes rigides tout en préservant la clarté conceptuelle.

Zia adopte une philosophie de **typage dynamique intelligent** : vous n'avez pas besoin de déclarer explicitement le type de vos données, mais le langage comprend intuitivement leur nature et adapte son comportement en conséquence. C'est la liberté dans la structure, l'élégance dans la simplicité.

---

## 🔤 Le mot magique : `var`

### **La déclaration universelle**

En Zia, tous les chemins mènent à `var` - ce mot-clé unique qui ouvre les portes de l'univers des données :

```zia
var nom = "Sophie";          // Zia comprend : "C'est du texte"
var âge = 28;                // Zia comprend : "C'est un nombre entier"
var taille = 1.68;           // Zia comprend : "C'est un nombre décimal"
var estMariée = vrai;        // Zia comprend : "C'est une valeur logique"
var enfants = nul;           // Zia comprend : "C'est une absence de valeur"
```

Cette simplicité cache une sophistication remarquable : Zia analyse la valeur que vous assignez et détermine automatiquement le type le plus approprié. C'est ce qu'on appelle l'**inférence de type** - votre programme devient plus intelligent et vous, plus libre.

---

## 🔢 Les nombres : l'arithmétique française

### **Entiers : la précision absolue**

Les nombres entiers en Zia représentent les quantités exactes, sans approximation :

```zia
var populationParis = 2175000;        // Population parisienne
var annéeNaissance = 1995;            // Une année précise
var températureHiver = -12;           // Températures négatives supportées
var compteurVisites = 0;              // Point de départ neutre
```

### **Nombres décimaux : la nuance mathématique**

Pour les valeurs qui demandent de la précision fractionnaire :

```zia
var pi = 3.14159265;                  // Constante mathématique
var tauxTVA = 20.0;                   // Pourcentage avec décimale
var moyenneClasse = 14.7;             // Note scolaire française
var prixEssence = 1.89;               // Prix au litre
```

### **Opérations : l'algèbre naturelle**

Zia gère intelligemment les conversions automatiques :

```zia
var base = 10;              // Entier
var coefficient = 1.5;      // Décimal
var résultat = base * coefficient;  // Résultat : 15.0 (décimal)

afficher "Calcul intelligent : " + résultat;
```

---

## 📝 Chaînes de caractères : l'expression textuelle

### **Déclaration : donner vie aux mots**

```zia
var prénom = "Marie-Claire";          // Prénoms composés français
var ville = "Aix-en-Provence";        // Villes avec tirets
var message = "Bonjour à tous !";     // Phrases complètes
var email = "contact@exemple.fr";     // Adresses électroniques
```

### **Caractères spéciaux : la richesse française**

Zia embrasse pleinement l'identité française :

```zia
var poème = "Être ou ne pas être, telle est la question";
var adresse = "123 rue de la Paix, 75001 Paris";
var exclamation = "Quelle magnifique journée !";
var question = "Comment allez-vous aujourd'hui ?";
```

### **Manipulation textuelle avancée**

```zia
var nom = "Dubois";
var prénom = "Jean";
var nomComplet = prénom + " " + nom;    // Concaténation élégante

var présentation = "Je m'appelle " + nomComplet + " et j'ai " + 35 + " ans.";
afficher présentation;
```

### **Textes multi-lignes et échappement**

```zia
var citation = "Victor Hugo a dit : \"L'avenir appartient aux enfants.\"";
var chemin = "C:\\Documents\\Projets\\MonFichier.txt";
var poèmeCourt = "Roses sont rouges,\nViolettes sont bleues,\nZia est français,\nEt c'est merveilleux !";
```

---

## ✅ Valeurs logiques : la vérité française

### **Vrai et faux : la clarté binaire**

```zia
var estMajeur = vrai;                 // Majorité légale
var aUnPermis = faux;                 // Situation administrative
var estConnecté = vrai;               // État de connexion
var estValide = faux;                 // Validation de données
```

### **Logique contextuelle**

Zia comprend naturellement les contextes logiques français :

```zia
var âge = 17;
var estMajeur = (âge >= 18);          // Évaluation automatique à 'faux'

var note = 16;
var estReçu = (note >= 10);           // Évaluation automatique à 'vrai'

var temps = "ensoleillé";
var peutSortir = (temps == "ensoleillé");  // Évaluation contextuelle
```

---

## ⭕ La nullité : l'art de l'absence

### **`nul` : quand rien vaut quelque chose**

```zia
var résultat = nul;                   // En attente de calcul
var utilisateur = nul;                // Aucun utilisateur connecté
var erreur = nul;                     // Pas d'erreur détectée
```

### **Gestion intelligente de la nullité**

```zia
var données = nul;

si (données != nul) {
    afficher "Données disponibles : " + données;
} sinon {
    afficher "Aucune donnée n'a été trouvée.";
}
```

---

## 🔄 Conversion et transformation : la fluidité des types

### **Conversions automatiques intelligentes**

Zia excelle dans l'art de la conversion contextuelle :

```zia
var nombre = 42;
var texte = "Le nombre magique est : " + nombre;  // Conversion automatique
afficher texte;  // Affiche : "Le nombre magique est : 42"

var prix = 29.99;
var message = "Prix : " + prix + "€";             // Conversion fluide
afficher message;  // Affiche : "Prix : 29.99€"
```

### **Conversions explicites avancées**

```zia
var texteNombre = "123";
var nombre = parseInt(texteNombre);    // Conversion texte vers entier
var decimal = parseFloat("45.67");     // Conversion texte vers décimal

var boolean = Boolean(1);              // Conversion vers booléen (vrai)
var chaîne = String(789);              // Conversion vers texte
```

---

## 🎯 Exemples pratiques avancés

### **Gestion d'un profil utilisateur**

```zia
fonction créerProfil(nom, prénom, âge, email) {
    var profil = {};
    
    profil.nomComplet = prénom + " " + nom;
    profil.âge = âge;
    profil.estMajeur = (âge >= 18);
    profil.email = email;
    profil.dateInscription = "2024-05-29";
    profil.estActif = vrai;
    
    retourner profil;
}

var utilisateur = créerProfil("Martin", "Sophie", 25, "sophie.martin@email.fr");
afficher "Bienvenue " + utilisateur.nomComplet + " !";
```

### **Calculatrice financière française**

```zia
fonction calculerTTC(prixHT, tauxTVA) {
    var montantTVA = prixHT * (tauxTVA / 100);
    var prixTTC = prixHT + montantTVA;
    
    afficher "Prix HT : " + prixHT + "€";
    afficher "TVA (" + tauxTVA + "%) : " + montantTVA + "€";
    afficher "Prix TTC : " + prixTTC + "€";
    
    retourner prixTTC;
}

var produit = {
    nom: "Ordinateur portable",
    prixHT: 1000.0,
    taux: 20.0
};

var total = calculerTTC(produit.prixHT, produit.taux);
```

### **Système de notation française**

```zia
fonction évaluerNote(note) {
    var appréciation = "";
    var estReçu = faux;
    
    si (note >= 16) {
        appréciation = "Très bien";
        estReçu = vrai;
    } sinon si (note >= 14) {
        appréciation = "Bien";
        estReçu = vrai;
    } sinon si (note >= 12) {
        appréciation = "Assez bien";
        estReçu = vrai;
    } sinon si (note >= 10) {
        appréciation = "Passable";
        estReçu = vrai;
    } sinon {
        appréciation = "Insuffisant";
        estReçu = faux;
    }
    
    var résultat = {
        note: note,
        appréciation: appréciation,
        estReçu: estReçu,
        mention: (note >= 14) ? "Avec mention" : "Sans mention"
    };
    
    retourner résultat;
}

var étudiant = "Pierre Durand";
var noteMaths = 15.5;
var évaluation = évaluerNote(noteMaths);

afficher étudiant + " - Note : " + évaluation.note + "/20";
afficher "Appréciation : " + évaluation.appréciation;
afficher "Résultat : " + (évaluation.estReçu ? "ADMIS" : "AJOURNE");
```

---

## 🔍 Vérification et validation de types

### **Fonctions d'introspection**

Zia propose des outils élégants pour examiner vos données :

```zia
fonction analyserVariable(variable, nom) {
    afficher "=== Analyse de " + nom + " ===";
    afficher "Valeur : " + variable;
    afficher "Type : " + typeof(variable);
    afficher "Est null : " + (variable == nul ? "oui" : "non");
    afficher "Est défini : " + (variable != undefined ? "oui" : "non");
    afficher "";
}

var exemples = [
    42,
    "Bonjour",
    vrai,
    nul,
    3.14
];

pour (var i = 0; i < exemples.length; i++) {
    analyserVariable(exemples[i], "Variable " + (i + 1));
}
```

---

## 🎨 Bonnes pratiques françaises

### **Nommage expressif et culturel**

```zia
// ✅ Nommage français expressif
var compteurVisiteurs = 0;
var listePrénoms = ["Marie", "Pierre", "Sophie"];
var estAuthentifié = faux;
var moyenneGénérale = 14.5;

// ✅ Contexte culturel français
var départements = ["Paris", "Lyon", "Marseille"];
var joursOuvrés = 5;
var congésPayés = 25;
var salaireMinimum = 1709.28;

// ❌ Éviter les anglicismes inutiles
var userCount = 0;           // Préférer : compteurUtilisateurs
var isValid = faux;          // Préférer : estValide
var firstName = "Marie";     // Préférer : prénom
```

### **Documentation et commentaires**

```zia
/**
 * Calcule l'âge d'une personne à partir de son année de naissance
 * @param {number} annéeNaissance - L'année de naissance
 * @return {number} L'âge calculé
 */
fonction calculerÂge(annéeNaissance) {
    var annéeActuelle = 2024;  // À adapter selon les besoins
    var âge = annéeActuelle - annéeNaissance;
    
    // Vérification de cohérence
    si (âge < 0 ou âge > 150) {
        afficher "Attention : âge incohérent calculé";
        retourner nul;
    }
    
    retourner âge;
}
```

---

## 🌟 La philosophie du typage dynamique français

### **Liberté et responsabilité**

Le typage dynamique de Zia reflète l'esprit français : il vous fait confiance tout en vous guidant intelligemment. Vous n'êtes pas contraint par des déclarations rigides, mais vous bénéficiez d'un système qui comprend vos intentions et adapte son comportement.

### **Élégance et pragmatisme**

Cette approche permet d'écrire du code élégant et naturel :

```zia
var données = nul;

// Plus tard dans le programme...
données = "Informations textuelles";

// Encore plus tard...
données = 42;

// Zia s'adapte gracieusement à chaque changement
afficher "Données actuelles : " + données;
```

---

## 🚀 Vers la maîtrise des données

La gestion des variables et types en Zia transcende la simple manipulation technique pour devenir un art de l'expression française structurée. Chaque `var` que vous écrivez est une déclaration d'intention, chaque valeur assignée raconte une partie de votre histoire algorithmique.

En maîtrisant ces concepts, vous acquérez la capacité de modéliser n'importe quelle réalité dans l'univers numérique, tout en conservant la clarté et l'élégance qui caractérisent la pensée française.

Dans le prochain chapitre, nous découvrirons comment ces données prennent vie à travers les structures de contrôle, ces mécanismes qui permettent à vos programmes de prendre des décisions et de s'adapter aux situations.

---

*Les variables sont vos mots, les types sont votre grammaire, et ensemble, ils forment le vocabulaire de votre créativité programmatique française.*