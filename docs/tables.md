# Générateur de Tables
## Maîtrisez les boucles et le formatage

Dans ce projet, vous allez créer un générateur de tables mathématiques (multiplication, addition, suites) en utilisant les boucles et les fonctions. C'est parfait pour maîtriser les boucles `pour` et `tantque` en Zia.

---

## 🎯 Objectifs du projet

### Ce que vous allez apprendre
- **Boucles `pour`** avec compteurs
- **Boucles `tantque`** avec conditions
- **Formatage de sortie** aligné et lisible
- **Fonctions paramétrées** pour la réutilisabilité
- **Suites mathématiques** et progressions

---

## 🚀 Code complet

```zia
// === GÉNÉRATEUR DE TABLES ZIA ===
// Démonstration des boucles et suites mathématiques

// Table de multiplication classique
fonction table_multiplication(nombre, limite) {
    afficher "=== TABLE DE ", nombre, " ===\n";
    
    pour (var i = 1; i <= limite; i = i + 1) {
        var resultat = nombre * i;
        afficher nombre, " x ", i, " = ", resultat, "\n";
    }
    
    afficher "\n";
}

// Table d'addition
fonction table_addition(nombre, limite) {
    afficher "=== TABLE D'ADDITION (+ ", nombre, ") ===\n";
    
    pour (var i = 1; i <= limite; i = i + 1) {
        var resultat = nombre + i;
        afficher nombre, " + ", i, " = ", resultat, "\n";
    }
    
    afficher "\n";
}

// Suites arithmétiques
fonction suite_arithmetique(premier, raison, nb_termes) {
    afficher "=== SUITE ARITHMÉTIQUE ===\n";
    afficher "Premier terme: ", premier, ", Raison: ", raison, "\n";
    afficher "Les ", nb_termes, " premiers termes:\n";
    
    var terme_actuel = premier;
    
    pour (var i = 1; i <= nb_termes; i = i + 1) {
        afficher "Terme ", i, ": ", terme_actuel, "\n";
        terme_actuel = terme_actuel + raison;
    }
    
    afficher "\n";
}

// Suites géométriques
fonction suite_geometrique(premier, raison, nb_termes) {
    afficher "=== SUITE GÉOMÉTRIQUE ===\n";
    afficher "Premier terme: ", premier, ", Raison: ", raison, "\n";
    afficher "Les ", nb_termes, " premiers termes:\n";
    
    var terme_actuel = premier;
    
    pour (var i = 1; i <= nb_termes; i = i + 1) {
        afficher "Terme ", i, ": ", terme_actuel, "\n";
        terme_actuel = terme_actuel * raison;
    }
    
    afficher "\n";
}

// Suite de Fibonacci
fonction suite_fibonacci(nb_termes) {
    afficher "=== SUITE DE FIBONACCI ===\n";
    afficher "Les ", nb_termes, " premiers termes:\n";
    
    si (nb_termes >= 1) {
        var a = 0;
        var b = 1;
        
        afficher "Terme 1: ", a, "\n";
        
        si (nb_termes >= 2) {
            afficher "Terme 2: ", b, "\n";
            
            pour (var i = 3; i <= nb_termes; i = i + 1) {
                var c = a + b;
                afficher "Terme ", i, ": ", c, "\n";
                a = b;
                b = c;
            }
        }
    }
    
    afficher "\n";
}

// Table des carrés et cubes
fonction table_puissances(limite) {
    afficher "=== TABLE DES CARRÉS ET CUBES ===\n";
    afficher "Nombre | Carré | Cube\n";
    afficher "-------|-------|------\n";
    
    pour (var i = 1; i <= limite; i = i + 1) {
        var carre = i * i;
        var cube = i * i * i;
        afficher "   ", i, "   |  ", carre, "   | ", cube, "\n";
    }
    
    afficher "\n";
}

// Factorielles
fonction table_factorielles(limite) {
    afficher "=== TABLE DES FACTORIELLES ===\n";
    
    var factorielle = 1;
    
    pour (var i = 1; i <= limite; i = i + 1) {
        factorielle = factorielle * i;
        afficher i, "! = ", factorielle, "\n";
    }
    
    afficher "\n";
}

// Nombres premiers (algorithme simple)
fonction afficher_nombres_premiers(limite) {
    afficher "=== NOMBRES PREMIERS jusqu'à ", limite, " ===\n";
    
    pour (var nombre = 2; nombre <= limite; nombre = nombre + 1) {
        var est_premier = vrai;
        
        // Vérifier si le nombre est premier
        pour (var diviseur = 2; diviseur < nombre; diviseur = diviseur + 1) {
            si (nombre % diviseur == 0) {
                est_premier = faux;
            }
        }
        
        si (est_premier) {
            afficher nombre, " est premier\n";
        }
    }
    
    afficher "\n";
}

// Tables de conversion
fonction table_conversion_temperature() {
    afficher "=== CONVERSION CELSIUS ↔ FAHRENHEIT ===\n";
    afficher "Celsius | Fahrenheit\n";
    afficher "--------|----------\n";
    
    pour (var celsius = 0; celsius <= 50; celsius = celsius + 10) {
        var fahrenheit = (celsius * 9 / 5) + 32;
        afficher "  ", celsius, "°C  |   ", fahrenheit, "°F\n";
    }
    
    afficher "\n";
}

// Démonstration avec boucle tantque
fonction compte_rebours(depart) {
    afficher "=== COMPTE À REBOURS depuis ", depart, " ===\n";
    
    var compteur = depart;
    
    tantque (compteur >= 0) {
        si (compteur == 0) {
            afficher "🚀 DÉCOLLAGE !\n";
        } sinon {
            afficher "T-", compteur, "\n";
        }
        compteur = compteur - 1;
    }
    
    afficher "\n";
}

// Programme principal
fonction demonstration_tables() {
    afficher "📊 Bienvenue dans le Générateur de Tables Zia !\n";
    afficher "\n";
    
    // Tables de multiplication
    table_multiplication(7, 10);
    table_multiplication(12, 5);
    
    // Table d'addition
    table_addition(25, 5);
    
    // Suites mathématiques
    suite_arithmetique(5, 3, 6);      // 5, 8, 11, 14...
    suite_geometrique(2, 3, 5);       // 2, 6, 18, 54...
    
    // Suite de Fibonacci
    suite_fibonacci(8);
    
    // Tables avancées
    table_puissances(6);
    table_factorielles(5);
    
    // Nombres premiers
    afficher_nombres_premiers(15);
    
    // Conversions
    table_conversion_temperature();
    
    // Démonstration boucle tantque
    compte_rebours(5);
    
    afficher "✅ Toutes les démonstrations terminées !\n";
    afficher "Vous maîtrisez maintenant les boucles en Zia !\n";
}

// Lancement du programme
demonstration_tables();
```

---

## 📊 Sortie attendue (extraits)

```
📊 Bienvenue dans le Générateur de Tables Zia !

=== TABLE DE 7 ===
7 x 1 = 7
7 x 2 = 14
7 x 3 = 21
7 x 4 = 28
7 x 5 = 35
7 x 6 = 42
7 x 7 = 49
7 x 8 = 56
7 x 9 = 63
7 x 10 = 70

=== TABLE DE 12 ===
12 x 1 = 12
12 x 2 = 24
12 x 3 = 36
12 x 4 = 48
12 x 5 = 60

=== TABLE D'ADDITION (+ 25) ===
25 + 1 = 26
25 + 2 = 27
25 + 3 = 28
25 + 4 = 29
25 + 5 = 30

=== SUITE ARITHMÉTIQUE ===
Premier terme: 5, Raison: 3
Les 6 premiers termes:
Terme 1: 5
Terme 2: 8
Terme 3: 11
Terme 4: 14
Terme 5: 17
Terme 6: 20

=== SUITE GÉOMÉTRIQUE ===
Premier terme: 2, Raison: 3
Les 5 premiers termes:
Terme 1: 2
Terme 2: 6
Terme 3: 18
Terme 4: 54
Terme 5: 162

=== SUITE DE FIBONACCI ===
Les 8 premiers termes:
Terme 1: 0
Terme 2: 1
Terme 3: 1
Terme 4: 2
Terme 5: 3
Terme 6: 5
Terme 7: 8
Terme 8: 13

=== TABLE DES CARRÉS ET CUBES ===
Nombre | Carré | Cube
-------|-------|------
   1   |  1    | 1
   2   |  4    | 8
   3   |  9    | 27
   4   |  16   | 64
   5   |  25   | 125
   6   |  36   | 216

=== TABLE DES FACTORIELLES ===
1! = 1
2! = 2
3! = 6
4! = 24
5! = 120

=== NOMBRES PREMIERS jusqu'à 15 ===
2 est premier
3 est premier
5 est premier
7 est premier
11 est premier
13 est premier

=== CONVERSION CELSIUS ↔ FAHRENHEIT ===
Celsius | Fahrenheit
--------|----------
  0°C  |   32°F
  10°C  |   50°F
  20°C  |   68°F
  30°C  |   86°F
  40°C  |   104°F
  50°C  |   122°F

=== COMPTE À REBOURS depuis 5 ===
T-5
T-4
T-3
T-2
T-1
🚀 DÉCOLLAGE !

✅ Toutes les démonstrations terminées !
Vous maîtrisez maintenant les boucles en Zia !
```

---

## 🎯 Concepts démontrés

### Types de boucles
- ✅ **Boucle `pour`** : Compteurs fixes, itérations contrôlées
- ✅ **Boucle `tantque`** : Conditions dynamiques
- ✅ **Boucles imbriquées** : Pour les nombres premiers

### Applications mathématiques
- ✅ **Tables classiques** : Multiplication, addition
- ✅ **Suites** : Arithmétiques, géométriques, Fibonacci
- ✅ **Calculs avancés** : Factorielles, puissances
- ✅ **Algorithmes** : Test de primalité simple

### Formatage et présentation
- ✅ **Alignement** : Tables avec colonnes
- ✅ **Séparateurs** : Lignes de démarcation
- ✅ **Formatage conditionnel** : Cas spéciaux

---

## 🚀 Défis d'amélioration

1. **Ajoutez d'autres suites** :
   ```zia
   // Suite des nombres triangulaires
   fonction suite_triangulaire(nb_termes) {
       pour (var i = 1; i <= nb_termes; i = i + 1) {
           var triangulaire = (i * (i + 1)) / 2;
           afficher "T", i, " = ", triangulaire;
       }
   }
   ```

2. **Tables de conversion** : distances, poids, devises

3. **Calculs statistiques** : moyennes mobiles, écarts

---

## 🎓 Ce que vous avez appris

- ✅ **Maîtrise des boucles** `pour` et `tantque`
- ✅ **Formatage professionnel** de la sortie
- ✅ **Suites mathématiques** et algorithmes
- ✅ **Optimisation** des calculs répétitifs
- ✅ **Modularité** : fonctions réutilisables

**Prochaine étape** : [📊 Analyseur de données](analyseur.md)# Générateur de Tables
