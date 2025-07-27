# Calculatrice Simple
## Démonstration des fonctions arithmétiques

Dans ce projet, vous allez créer des fonctions de calcul et démontrer leur utilisation avec différents exemples. C'est un excellent premier projet pour maîtriser les fonctions et la gestion d'erreurs en Zia.

---

## 🎯 Objectifs du projet

### Ce que vous allez apprendre
- **Créer des fonctions** modulaires et réutilisables
- **Gérer les erreurs** comme la division par zéro
- **Organiser le code** de manière claire
- **Tester des fonctions** avec différentes valeurs
- **Formater la sortie** de manière professionnelle

---

## 🚀 Code complet

```zia
// === CALCULATRICE ZIA ===
// Démonstration des opérations arithmétiques de base

// Fonctions d'opérations
fonction additionner(a, b) {
    retourner a + b;
}

fonction soustraire(a, b) {
    retourner a - b;
}

fonction multiplier(a, b) {
    retourner a * b;
}

fonction diviser(a, b) {
    si (b == 0) {
        afficher "\nERREUR: Division par zéro impossible !\n";
        retourner 0;
    }
    retourner a / b;
}

// Fonction pour afficher un calcul
fonction afficher_calcul(a, operation, b, resultat) {
    afficher a, " ", operation, " ", b, " = ", resultat, "\n";
}

// Fonction pour tester une opération
fonction tester_operation(a, b, op) {
    var resultat = 0;
    var symbole = "";

    si (op == 1) {
        resultat = additionner(a, b);
        symbole = "+";
    } sinon si (op == 2) {
        resultat = soustraire(a, b);
        symbole = "-";
    } sinon si (op == 3) {
        resultat = multiplier(a, b);
        symbole = "*";
    } sinon si (op == 4) {
        si (b != 0) {
            resultat = diviser(a, b);
            symbole = "/";
            afficher_calcul(a, symbole, b, resultat);
        } sinon {
            afficher a, " / ", b, " = ERREUR (division par zéro)\n";
        }
        retourner;
    }

    afficher_calcul(a, symbole, b, resultat);
}

// Démonstration avec différentes valeurs
fonction demonstration_calculatrice() {
    afficher "\n====== CALCULATRICE ZIA ======\n";
    afficher "\nDémonstration des opérations de base\n";

    // Test avec des nombres positifs
    afficher "\n--- Tests avec nombres positifs ---\n";
    tester_operation(15, 7, 1);  // Addition
    tester_operation(15, 7, 2);  // Soustraction
    tester_operation(15, 7, 3);  // Multiplication
    tester_operation(15, 7, 4);  // Division

    // Test avec des nombres négatifs
    afficher "\n--- Tests avec nombres négatifs ---\n";
    tester_operation(-10, 5, 1);
    tester_operation(-10, 5, 2);
    tester_operation(-10, 5, 3);
    tester_operation(-10, 5, 4);

    // Test avec des décimaux
    afficher "\n--- Tests avec décimaux ---\n";
    tester_operation(7.5, 2.5, 1);
    tester_operation(7.5, 2.5, 2);
    tester_operation(7.5, 2.5, 3);
    tester_operation(7.5, 2.5, 4);

    // Test de la division par zéro
    afficher "\n--- Test gestion d'erreurs ---\n";
    tester_operation(10, 0, 4);  // Division par zéro
    tester_operation(0, 5, 4);   // Zéro divisé par un nombre

    // Calculs complexes
    afficher "\n--- Calculs combinés ---\n";
    var a = 10;
    var b = 5;
    var c = 2;

    var resultat1 = additionner(a, b);
    var resultat2 = multiplier(resultat1, c);
    afficher "\n((", a, " + ", b, ") * ", c, ") = ", resultat2;

    var resultat3 = diviser(a, b);
    var resultat4 = additionner(resultat3, c);
    afficher "\n((", a, " / ", b, ") + ", c, ") = ", resultat4;
}

// Fonction pour calculer l'aire et périmètre
fonction geometrie_rectangle(longueur, largeur) {
    afficher "\n=== Calculs géométriques ===\n";
    afficher "\nRectangle: ", longueur, " x ", largeur;

    var aire = multiplier(longueur, largeur);
    afficher "\nAire: ", aire, " unités²";

    var perimetre = multiplier(additionner(longueur, largeur), 2);
    afficher "\nPérimètre: ", perimetre, " unités";
}

// Fonction pour calculer des pourcentages
fonction calculer_pourcentages() {
    afficher "\n=== Calculs de pourcentages ===\n";

    var prix_initial = 100;
    var taux_remise = 20;

    afficher "\nPrix initial: ", prix_initial, "€";
    afficher "\nRemise: ", taux_remise, "%";

    var montant_remise = diviser(multiplier(prix_initial, taux_remise), 100);
    var prix_final = soustraire(prix_initial, montant_remise);

    afficher "\nMontant de la remise: ", montant_remise, "€";
    afficher "\nPrix final: ", prix_final, "€";
}

// Programme principal
afficher "\n🧮 Bienvenue dans la démonstration de la Calculatrice Zia !\n";

demonstration_calculatrice();

geometrie_rectangle(12, 8);

calculer_pourcentages();

afficher "\n✅ Démonstration terminée !";
afficher "\nVous avez vu comment créer et utiliser des fonctions arithmétiques en Zia.";
```

---

## 📊 Sortie attendue

```
========== Code En Compilation ==========

🧮 Bienvenue dans la démonstration de la Calculatrice Zia !
====== CALCULATRICE ZIA ======
Démonstration des opérations de base
--- Tests avec nombres positifs ---
15 + 7 = 22
15 - 7 = 8
15 * 7 = 105
15 / 7 = 2.14286
--- Tests avec nombres négatifs ---
-10 + 5 = -5
-10 - 5 = -15
-10 * 5 = -50
-10 / 5 = -2
--- Tests avec décimaux ---
7.5 + 2.5 = 10
7.5 - 2.5 = 5
7.5 * 2.5 = 18.75
7.5 / 2.5 = 3
--- Test gestion d'erreurs ---
10 / 0 = ERREUR (division par zéro)
0 / 5 = 0
--- Calculs combinés ---
((10 + 5) * 2) = 30
((10 / 5) + 2) = 4
=== Calculs géométriques ===
Rectangle: 12 x 8
Aire: 96 unités²
Périmètre: 40 unités
=== Calculs de pourcentages ===
Prix initial: 100€
Remise: 20%
Montant de la remise: 20€
Prix final: 80€
✅ Démonstration terminée !
Vous avez vu comment créer et utiliser des fonctions arithmétiques en Zia.
```

---

## 🎯 Points clés du projet

### Fonctions créées
- ✅ `additionner(a, b)` - Addition de deux nombres
- ✅ `soustraire(a, b)` - Soustraction 
- ✅ `multiplier(a, b)` - Multiplication
- ✅ `diviser(a, b)` - Division avec gestion d'erreur
- ✅ `afficher_calcul()` - Formatage de la sortie
- ✅ `tester_operation()` - Tests automatisés

### Concepts démontrés
- **Gestion d'erreurs** : Division par zéro
- **Réutilisabilité** : Mêmes fonctions pour différents cas
- **Organisation** : Code structuré et lisible
- **Tests variés** : Positifs, négatifs, décimaux
- **Applications pratiques** : Géométrie, pourcentages

---

## 🚀 Défis d'amélioration

1. **Ajoutez d'autres opérations** :
   ```zia
   fonction puissance(base, exposant) {
       // Calculer base^exposant avec une boucle
   }
   
   ```

2. **Calculateur de conversions** : mètres/pieds, euros/dollars, etc.

---

## 🎓 Ce que vous avez appris

- ✅ **Structuration du code** en fonctions logiques
- ✅ **Gestion des erreurs** avec des conditions
- ✅ **Tests automatisés** de vos fonctions
- ✅ **Formatage de sortie** professionnel
- ✅ **Applications pratiques** des mathématiques

**Prochaine étape** : [🎲 Générateur de tables](tables.md)