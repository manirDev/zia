# Opérateurs Arithmétiques et Logiques
## L'algèbre française en programmation

> *"Les opérateurs sont les verbes de votre langage algorithmique : ils donnent du mouvement à vos données et transforment vos intentions en actions."*

---

## 🔢 Opérateurs Arithmétiques : l'art du calcul

### **Les quatre opérations fondamentales**

Zia respecte les conventions mathématiques françaises tout en offrant une syntaxe claire et intuitive :

```zia
var a = 15;
var b = 4;

var addition = a + b;        // 19 - L'union des nombres
var soustraction = a - b;    // 11 - La différence révélée
var multiplication = a * b;  // 60 - La puissance du produit
var division = a / b;        // 3.75 - La précision du quotient
```

### **Division : intelligence contextuelle**

Zia gère intelligemment les divisions selon le contexte français :

```zia
var prixTotal = 100;
var nombrePersonnes = 3;

var partParPersonne = prixTotal / nombrePersonnes;  // 33.333... (décimal automatique)
afficher "Chaque personne paie : " + partParPersonne + "€";

// Division entière explicite
var quotientEntier = Math.floor(prixTotal / nombrePersonnes);  // 33
var reste = prixTotal % nombrePersonnes;                       // 1
afficher "Division : " + quotientEntier + " reste " + reste;
```

### **Modulo : le reste français**

L'opérateur modulo (`%`) révèle le reste d'une division :

```zia
var année = 2024;
var estBissextile = (année % 4 == 0) et (année % 100 != 0 ou année % 400 == 0);

var numéroSemaine = 15;
var jourSemaine = numéroSemaine % 7;  // 0=Lundi, 1=Mardi, etc.

var nombre = 17;
var estPair = (nombre % 2 == 0);      // faux
var estImpair = (nombre % 2 == 1);    // vrai
```

### **Puissance et racines : mathématiques avancées**

```zia
var base = 2;
var exposant = 8;
var puissance = base ** exposant;     // 256 - Opérateur puissance moderne

// Ou avec la fonction Math
var racineCarrée = Math.sqrt(16);     // 4
var racineNCubique = Math.cbrt(27);   // 3
var logarithme = Math.log10(1000);    // 3
```

---

## 📊 Opérateurs d'assignation : l'élégance de l'attribution

### **Assignation simple et composée**

```zia
var compteur = 0;

// Assignations composées - l'efficacité française
compteur += 5;    // compteur = compteur + 5  (maintenant 5)
compteur -= 2;    // compteur = compteur - 2  (maintenant 3)
compteur *= 3;    // compteur = compteur * 3  (maintenant 9)
compteur /= 3;    // compteur = compteur / 3  (maintenant 3)
compteur %= 2;    // compteur = compteur % 2  (maintenant 1)

afficher "Compteur final : " + compteur;
```

### **Incrémentation et décrémentation**

```zia
var visiteurs = 100;

visiteurs++;      // Post-incrémentation : visiteurs devient 101
++visiteurs;      // Pré-incrémentation : visiteurs devient 102

visiteurs--;      // Post-décrémentation : visiteurs devient 101
--visiteurs;      // Pré-décrémentation : visiteurs devient 100

// Utilisation dans des contextes
var i = 0;
var tableau = [1, 2, 3, 4, 5];

tant que (i < tableau.length) {
    afficher "Élément " + i + " : " + tableau[i];
    i++;  // Incrémentation pour avancer
}
```

---

## 🎯 Opérateurs de comparaison : l'art du jugement

### **Égalité et inégalité**

```zia
var âge = 18;
var majorité = 18;

// Égalité - la reconnaissance
var estMajeur = (âge == majorité);          // vrai
var nEstPasMajeur = (âge != majorité);      // faux

// Égalité stricte - la précision absolue
var nombre = 5;
var texte = "5";
var égalitéSouple = (nombre == texte);      // vrai (conversion automatique)
var égalitéStricte = (nombre === texte);    // faux (types différents)
var inégalitéStricte = (nombre !== texte);  // vrai (types différents)
```

### **Comparaisons numériques**

```zia
var note = 14.5;
var noteMoyenne = 10;
var noteExcellence = 16;

var estReçu = (note >= noteMoyenne);        // vrai
var estBrillant = (note > noteExcellence);  // faux
var peutMieux = (note < noteExcellence);    // vrai
var insuffisant = (note <= 8);             // faux

// Comparaisons textuelles (ordre alphabétique)
var prénom1 = "Alice";
var prénom2 = "Bernard";
var ordreAlphabétique = (prénom1 < prénom2);  // vrai
```

---

## 🧠 Opérateurs logiques : la logique française

### **ET, OU, NON : la trinité logique**

```zia
var âge = 25;
var aPermis = vrai;
var estAssurance = vrai;

// ET logique (&&) - toutes les conditions doivent être vraies
var peutConduire = (âge >= 18) et aPermis et estAssurance;  // vrai

// OU logique (||) - au moins une condition doit être vraie
var peutVoter = (âge >= 18) ou (âge >= 16 et estCitoyen);

// NON logique (!) - inversion de la valeur
var estMineur = non (âge >= 18);  // faux
var naPasPermis = non aPermis;    // faux
```

### **Logique avancée et court-circuit**

```zia
fonction vérifierAccès(utilisateur) {
    // Court-circuit : si utilisateur est nul, la seconde condition n'est pas évaluée
    retourner (utilisateur != nul) et (utilisateur.estActif) et (utilisateur.niveau >= 2);
}

fonction obtenirMessage(urgent, message) {
    // Opérateur OU avec valeur par défaut
    retourner urgent ou "Message standard";
}

var user = { nom: "Sophie", estActif: vrai, niveau: 3 };
var accès = vérifierAccès(user);  // vrai

var msg1 = obtenirMessage(faux, "Alerte");     // "Message standard"
var msg2 = obtenirMessage("Urgent!", "Test");  // "Urgent!"
```

---

## 🔀 Opérateurs ternaires : la concision élégante

### **Condition ? Vrai : Faux**

```zia
var âge = 17;
var statut = (âge >= 18) ? "Majeur" : "Mineur";
afficher "Statut : " + statut;  // "Statut : Mineur"

var note = 15;
var mention = (note >= 16) ? "Très bien" : 
              (note >= 14) ? "Bien" : 
              (note >= 12) ? "Assez bien" : 
              (note >= 10) ? "Passable" : "Insuffisant";

var temperature = 22;
var conseil = (temperature > 25) ? "Portez des vêtements légers" :
              (temperature > 15) ? "Une veste légère suffira" :
              "N'oubliez pas votre manteau";
```

---

## 🎨 Opérateurs sur les chaînes : la manipulation textuelle

### **Concaténation et interpolation**

```zia
var prénom = "Marie";
var nom = "Dupont";
var âge = 28;

// Concaténation classique
var présentation = "Je m'appelle " + prénom + " " + nom + " et j'ai " + âge + " ans.";

// Template literals (chaînes formatées)
var messageFormaté = `Bonjour ${prénom} ${nom}, vous avez ${âge} ans.`;

// Répétition de chaînes
var séparateur = "-".repeat(50);  // 50 tirets
var étoiles = "*".repeat(10);     // 10 étoiles
```

### **Comparaisons textuelles**

```zia
var motDePasse = "MonMotDePasse123";
var confirmation = "MonMotDePasse123";

var correspond = (motDePasse == confirmation);  // vrai

// Comparaison insensible à la casse
var ville1 = "PARIS";
var ville2 = "paris";
var mêmeVille = (ville1.toLowerCase() == ville2.toLowerCase());  // vrai

// Inclusion et recherche
var texte = "Bonjour tout le monde";
var contientMonde = texte.includes("monde");      // vrai
var commenceParBonjour = texte.startsWith("Bonjour");  // vrai
var finitParMonde = texte.endsWith("monde");     // vrai
```

---

## 🔢 Opérateurs bit à bit : la précision binaire

### **Manipulation des bits**

```zia
var a = 12;  // 1100 en binaire
var b = 5;   // 0101 en binaire

var etBinaire = a & b;      // 4 (0100)
var ouBinaire = a | b;      // 13 (1101)
var ouExclusif = a ^ b;     // 9 (1001)
var complement = ~a;        // -13 (complément à deux)

var décalageGauche = a << 2;   // 48 (110000)
var décalageDroite = a >> 2;   // 3 (0011)
```

---

## 📋 Exemples pratiques avancés

### **Calculatrice française complète**

```zia
fonction calculatriceFrançaise() {
    var opérandes = {
        a: 0,
        b: 0,
        opération: ""
    };
    
    fonction effectuerCalcul(a, b, op) {
        selon (op) {
            cas "+":
                retourner a + b;
            cas "-":
                retourner a - b;
            cas "*":
            cas "×":
                retourner a * b;
            cas "/":
            cas "÷":
                retourner (b != 0) ? a / b : "Erreur : Division par zéro";
            cas "%":
                retourner a % b;
            cas "**":
            cas "^":
                retourner a ** b;
            défaut:
                retourner "Opération inconnue";
        }
    }
    
    // Tests avec différents opérateurs
    var résultats = [
        effectuerCalcul(15, 3, "+"),    // 18
        effectuerCalcul(15, 3, "-"),    // 12
        effectuerCalcul(15, 3, "*"),    // 45
        effectuerCalcul(15, 3, "/"),    // 5
        effectuerCalcul(15, 3, "%"),    // 0
        effectuerCalcul(2, 8, "**")     // 256
    ];
    
    pour (var i = 0; i < résultats.length; i++) {
        afficher "Résultat " + (i + 1) + " : " + résultats[i];
    }
}

calculatriceFrançaise();
```

### **Système de validation française**

```zia
fonction validerDonnéesFrançaises(données) {
    var résultat = {
        estValide: vrai,
        erreurs: []
    };
    
    // Validation du nom (lettres, espaces, tirets, apostrophes)
    var nomValide = données.nom et 
                   données.nom.length >= 2 et
                   /^[a-zA-ZÀ-ÿ\s\-']+$/.test(données.nom);
    
    si (non nomValide) {
        résultat.estValide = faux;
        résultat.erreurs.push("Nom invalide");
    }
    
    // Validation de l'âge
    var âgeValide = données.âge et
                   typeof(données.âge) === "number" et
                   données.âge >= 0 et données.âge <= 150;
    
    si (non âgeValide) {
        résultat.estValide = faux;
        résultat.erreurs.push("Âge invalide");
    }
    
    // Validation email français
    var emailValide = données.email et
                     /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(données.email);
    
    si (non emailValide) {
        résultat.estValide = faux;
        résultat.erreurs.push("Email invalide");
    }
    
    // Validation code postal français
    var codePostalValide = données.codePostal et
                          /^[0-9]{5}$/.test(données.codePostal) et
                          données.codePostal >= "01000" et
                          données.codePostal <= "98999";
    
    si (non codePostalValide) {
        résultat.estValide = faux;
        résultat.erreurs.push("Code postal français invalide");
    }
    
    retourner résultat;
}

// Test du système
var utilisateur = {
    nom: "Marie-Claire Dubois",
    âge: 28,
    email: "marie.dubois@exemple.fr",
    codePostal: "75001"
};

var validation = validerDonnéesFrançaises(utilisateur);

si (validation.estValide) {
    afficher "✅ Données valides pour " + utilisateur.nom;
} sinon {
    afficher "❌ Erreurs détectées :";
    pour (var erreur de validation.erreurs) {
        afficher "  - " + erreur;
    }
}
```

### **Système de notation et mentions**

```zia
fonction évaluerPerformance(notes) {
    var statistiques = {
        moyenne: 0,
        médiane: 0,
        minimum: Number.MAX_VALUE,
        maximum: Number.MIN_VALUE,
        écartType: 0,
        mention: "",
        estAdmis: faux
    };
    
    // Calcul de la moyenne
    var somme = 0;
    pour (var note de notes) {
        somme += note;
        statistiques.minimum = Math.min(statistiques.minimum, note);
        statistiques.maximum = Math.max(statistiques.maximum, note);
    }
    
    statistiques.moyenne = somme / notes.length;
    
    // Calcul de la médiane
    var notesTriées = [...notes].sort((a, b) => a - b);
    var milieu = Math.floor(notesTriées.length / 2);
    
    si (notesTriées.length % 2 == 0) {
        statistiques.médiane = (notesTriées[milieu - 1] + notesTriées[milieu]) / 2;
    } sinon {
        statistiques.médiane = notesTriées[milieu];
    }
    
    // Calcul de l'écart-type
    var varianceSum = 0;
    pour (var note de notes) {
        varianceSum += (note - statistiques.moyenne) ** 2;
    }
    statistiques.écartType = Math.sqrt(varianceSum / notes.length);
    
    // Détermination de la mention
    var moy = statistiques.moyenne;
    statistiques.estAdmis = moy >= 10;
    
    statistiques.mention = (moy >= 18) ? "Très bien avec félicitations" :
                          (moy >= 16) ? "Très bien" :
                          (moy >= 14) ? "Bien" :
                          (moy >= 12) ? "Assez bien" :
                          (moy >= 10) ? "Passable" : "Insuffisant";
    
    retourner statistiques;
}

// Test avec des notes françaises
var notesÉlève = [14, 16, 12, 18, 15, 13, 17, 14];
var résultats = évaluerPerformance(notesÉlève);

afficher "=== Bulletin de Notes ===";
afficher "Moyenne générale : " + résultats.moyenne.toFixed(2) + "/20";
afficher "Médiane : " + résultats.médiane + "/20";
afficher "Note minimale : " + résultats.minimum + "/20";
afficher "Note maximale : " + résultats.maximum + "/20";
afficher "Écart-type : " + résultats.écartType.toFixed(2);
afficher "Mention : " + résultats.mention;
afficher "Résultat : " + (résultats.estAdmis ? "✅ ADMIS" : "❌ AJOURNÉ");
```

---

## 🎯 Priorité des opérateurs : l'ordre français

### **Hiérarchie complète des opérations**

La priorité des opérateurs en Zia suit une logique mathématique et française rigoureuse. Comprendre cette hiérarchie est essentiel pour écrire des expressions correctes et prévisibles.

```zia
// Tableau complet de priorité (1 = plus haute priorité)
```

| Niveau | Opérateur | Description | Associativité | Exemple |
|--------|-----------|-------------|---------------|---------|
| 1 | `()` `[]` `{}` | Groupement, accès | Gauche à droite | `(a + b)` `obj.prop` |
| 2 | `++` `--` (post) | Post-incrémentation | Gauche à droite | `a++` `b--` |
| 3 | `++` `--` (pré) `+` `-` `!` `non` `typeof` | Unaires | Droite à gauche | `++a` `-x` `!valid` |
| 4 | `**` | Puissance | Droite à gauche | `2 ** 3 ** 2` = `2 ** 9` |
| 5 | `*` `/` `%` | Multiplicatifs | Gauche à droite | `a * b / c` |
| 6 | `+` `-` | Additifs | Gauche à droite | `a + b - c` |
| 7 | `<<` `>>` `>>>` | Décalage bit | Gauche à droite | `a << 2` |
| 8 | `<` `<=` `>` `>=` | Relationnels | Gauche à droite | `a < b <= c` |
| 9 | `==` `!=` `===` `!==` | Égalité | Gauche à droite | `a == b != c` |
| 10 | `&` | ET bit à bit | Gauche à droite | `a & b & c` |
| 11 | `^` | OU exclusif bit | Gauche à droite | `a ^ b ^ c` |
| 12 | `\|` | OU bit à bit | Gauche à droite | `a \| b \| c` |
| 13 | `&&` `et` | ET logique | Gauche à droite | `a et b et c` |
| 14 | `\|\|` `ou` | OU logique | Gauche à droite | `a ou b ou c` |
| 15 | `? :` | Ternaire | Droite à gauche | `a ? b : c ? d : e` |
| 16 | `=` `+=` `-=` `*=` `/=` `%=` | Assignation | Droite à gauche | `a = b += c` |
| 17 | `,` | Virgule | Gauche à droite | `a, b, c` |

### **Exemples détaillés avec évaluation étape par étape**

```zia
// Exemple 1 : Arithmétique mixte
var résultat1 = 2 + 3 * 4 ** 2;
// Étape 1 : 4 ** 2 = 16        (puissance en premier)
// Étape 2 : 3 * 16 = 48        (multiplication)
// Étape 3 : 2 + 48 = 50        (addition)
// Résultat : 50

// Exemple 2 : Comparaisons et logique
var résultat2 = 5 + 3 > 7 et 10 / 2 == 5;
// Étape 1 : 5 + 3 = 8          (addition)
// Étape 2 : 10 / 2 = 5         (division)
// Étape 3 : 8 > 7 = vrai       (comparaison)
// Étape 4 : 5 == 5 = vrai      (égalité)
// Étape 5 : vrai et vrai = vrai (ET logique)
// Résultat : vrai

// Exemple 3 : Assignations composées
var a = 5;
var b = 2;
var c = a += b *= 3;
// Étape 1 : b *= 3 → b = 2 * 3 = 6    (assignation droite à gauche)
// Étape 2 : a += 6 → a = 5 + 6 = 11   (assignation)
// Étape 3 : c = 11                     (assignation finale)
// Résultats : a=11, b=6, c=11
```

### **Associativité : l'ordre d'évaluation**

```zia
// Associativité gauche à droite (la plupart des opérateurs)
var calcul1 = 10 - 5 - 2;
// Évaluation : (10 - 5) - 2 = 5 - 2 = 3

var calcul2 = 20 / 4 / 2;
// Évaluation : (20 / 4) / 2 = 5 / 2 = 2.5

// Associativité droite à gauche (puissance, assignation, ternaire)
var puissance = 2 ** 3 ** 2;
// Évaluation : 2 ** (3 ** 2) = 2 ** 9 = 512

var assignation = a = b = c = 10;
// Évaluation : a = (b = (c = 10))
// Résultat : a=10, b=10, c=10

var ternaire = a > 0 ? b > 0 ? "double positif" : "a positif, b négatif" : "a négatif";
// Évaluation : a > 0 ? (b > 0 ? "double positif" : "a positif, b négatif") : "a négatif"
```

### **Cas complexes et pièges courants**

```zia
// Piège 1 : Incrémentation avec autres opérateurs
var x = 5;
var résultat = x++ + ++x;
// x++ utilise 5, puis x devient 6
// ++x incrémente x à 7, puis utilise 7
// résultat = 5 + 7 = 12
// x final = 7

// Piège 2 : Mélange logique et arithmétique
var test = 5 + 3 > 6 et 2 * 4 < 10 ou faux;
// Étape 1 : 5 + 3 = 8
// Étape 2 : 2 * 4 = 8
// Étape 3 : 8 > 6 = vrai
// Étape 4 : 8 < 10 = vrai
// Étape 5 : vrai et vrai = vrai
// Étape 6 : vrai ou faux = vrai

// Piège 3 : Ternaire imbriqué
var note = 15;
var mention = note >= 16 ? "Très bien" : 
              note >= 14 ? "Bien" : 
              note >= 12 ? "Assez bien" : "Passable";
// Associativité droite à gauche :
// note >= 16 ? "Très bien" : (note >= 14 ? "Bien" : (note >= 12 ? "Assez bien" : "Passable"))
// 15 >= 16 = faux, donc évalue la partie droite
// 15 >= 14 = vrai, donc résultat = "Bien"
```

### **Utilisation stratégique des parenthèses**

```zia
// Sans parenthèses - peut être ambigu
var calcul = prix * quantité + taxe / 100;
// Évaluation : prix * quantité + (taxe / 100)

// Avec parenthèses - intention claire
var calculClair = (prix * quantité) + (taxe / 100);
var calculAlternatif = prix * (quantité + taxe) / 100;

// Logique complexe clarifiée
var condition = (âge >= 18 et aPermis) ou (âge >= 16 et supervisé);
// vs
var conditionAmbiguë = âge >= 18 et aPermis ou âge >= 16 et supervisé;
// Cette dernière évalue comme : (âge >= 18 et aPermis) ou (âge >= 16 et supervisé)
// mais c'est moins clair

// Calculs financiers français
var montantTTC = (prixHT * quantité * (1 + tauxTVA / 100)) - remise;
var montantAvecFrais = ((prixHT + fraisLivraison) * quantité) * (1 + tauxTVA / 100);
```

### **Fonctions utilitaires pour déboguer la precedence**

```zia
fonction analyserExpression(expression, description) {
    afficher "=== " + description + " ===";
    afficher "Expression : " + expression;
    afficher "Résultat : " + eval(expression);
    afficher "";
}

// Tests de precedence
analyserExpression("2 + 3 * 4", "Addition et multiplication");
analyserExpression("(2 + 3) * 4", "Parenthèses modifiant la priorité");
analyserExpression("2 ** 3 ** 2", "Puissance associative droite");
analyserExpression("10 - 5 - 2", "Soustraction associative gauche");
analyserExpression("5 > 3 et 2 < 4", "Comparaisons et logique");

fonction démontrerPriorités() {
    var a = 2, b = 3, c = 4, d = 5;
    
    var tests = [
        { expr: "a + b * c", attendu: 2 + 3 * 4 },
        { expr: "(a + b) * c", attendu: (2 + 3) * 4 },
        { expr: "a < b et c > d", attendu: 2 < 3 && 4 > 5 },
        { expr: "a + b > c ou d == a + b", attendu: 2 + 3 > 4 || 5 == 2 + 3 },
        { expr: "++a + b++", attendu: function() { var x = 2, y = 3; return ++x + y++; }() }
    ];
    
    pour (var test de tests) {
        afficher `${test.expr} = ${test.attendu}`;
    }
}

démontrerPriorités();
```

---

## 🌟 Bonnes pratiques françaises

### **Lisibilité et expressivité**

```zia
// ✅ Utilisation claire des parenthèses
var prixTTC = (prixHT * (1 + tauxTVA / 100));

// ✅ Variables intermédiaires pour la clarté
var estMajeur = (âge >= 18);
var aPermis = (permis != nul);
var peutConduire = estMajeur et aPermis;

// ✅ Nommage français expressif
var estConnectéEtActif = (utilisateur.estConnecté et utilisateur.estActif);

// ❌ Éviter les expressions trop complexes
var résultat = ((a > b) ? (c + d) : (e - f)) * ((g < h) ? i : j);

// ✅ Préférer la décomposition
var condition1 = (a > b);
var valeur1 = condition1 ? (c + d) : (e - f);
var condition2 = (g < h);
var valeur2 = condition2 ? i : j;
var résultat = valeur1 * valeur2;
```

---

## 🚀 Vers la maîtrise opérationnelle

Les opérateurs en Zia ne sont pas de simples symboles : ils sont les outils qui donnent vie à vos données et transforment vos idées en logique exécutable. En maîtrisant leur utilisation, vous acquérez la capacité d'exprimer n'importe quelle transformation mathématique ou logique avec l'élégance et la précision qui caractérisent la programmation française.

Chaque opérateur a sa personnalité, sa priorité, et son rôle dans l'orchestre de votre code. Utilisez-les avec discernement, clarté, et n'hésitez jamais à privilégier la lisibilité sur la concision quand cela sert la compréhension.

---

*Les opérateurs sont les verbes de votre vocabulaire algorithmique : conjuguez-les avec sagesse pour créer des programmes qui chantent en français.*