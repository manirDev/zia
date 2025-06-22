# Structures de contrôle en Zia
## Maîtriser le flux d'exécution de vos programmes

> *"Le contrôle du flux d'un programme, c'est comme diriger un orchestre : chaque instruction doit arriver au bon moment, dans le bon ordre, pour créer l'harmonie parfaite."*

---

## 🎯 Introduction au contrôle de flux

Les structures de contrôle sont les fondations qui permettent à vos programmes Zia de prendre des décisions, de répéter des actions et de s'adapter aux différentes situations. Elles transforment une simple séquence d'instructions en un programme intelligent et réactif.

En Zia, le contrôle de flux respecte la logique française naturelle, rendant votre code aussi lisible qu'un texte bien structuré.

---

## 🔀 Structures conditionnelles : l'art de décider

### **La condition simple : `si-sinon`**

La structure conditionnelle la plus fondamentale permet à votre programme de choisir entre deux chemins :

```zia
var age = 18;

si (age >= 18) {
    afficher "Vous êtes majeur";
} sinon {
    afficher "Vous êtes mineur";
}
```

### **Conditions multiples : `si-sinon si-sinon`**

Pour des décisions plus nuancées, Zia propose une chaîne de conditions :

```zia
var note = 14;

si (note >= 16) {
    afficher "Mention très bien !";
} sinon si (note >= 14) {
    afficher "Mention bien !";
} sinon si (note >= 12) {
    afficher "Mention assez bien !";
} sinon si (note >= 10) {
    afficher "Admis !";
} sinon {
    afficher "Échec, courage pour la prochaine fois.";
}
```

### **Conditions complexes avec opérateurs logiques**

Combinez plusieurs conditions avec `et` et `ou` :

```zia
var temperature = 25;
var estEnsoleille = vrai;

si (temperature > 20 et estEnsoleille) {
    afficher "Parfait pour une sortie !";
} sinon si (temperature > 15 ou estEnsoleille) {
    afficher "Ça peut aller avec une veste.";
} sinon {
    afficher "Mieux vaut rester à l'intérieur.";
}
```

### **Conditions imbriquées : la précision dans la décision**

```zia
var estConnecte = vrai;
var estAdmin = faux;
var aAcces = vrai;

si (estConnecte) {
    afficher "Utilisateur connecté";
    
    si (estAdmin) {
        afficher "Accès administrateur accordé";
    } sinon {
        si (aAcces) {
            afficher "Accès utilisateur standard";
        } sinon {
            afficher "Accès refusé";
        }
    }
} sinon {
    afficher "Veuillez vous connecter";
}
```

---

## 🎛️ Sélection multiple : `selon-cas`

Quand vous avez plusieurs choix distincts, `selon-cas` offre une syntaxe plus claire que les conditions en chaîne :

### **Sélection simple**

```zia
var jourSemaine = 3;

selon (jourSemaine) {
    cas 1:
        afficher "Lundi - Nouveau départ !";
        quitter;
    
    cas 2:
        afficher "Mardi - On prend le rythme";
        quitter;
    
    cas 3:
        afficher "Mercredi - Milieu de semaine";
        quitter;
    
    cas 4:
        afficher "Jeudi - Bientôt le weekend";
        quitter;
    
    cas 5:
        afficher "Vendredi - Dernier effort !";
        quitter;
    
    defaut:
        afficher "Weekend - Temps libre !";
}
```

### **Sélection avec caractères**

```zia
var choixMenu = 'B';

selon (choixMenu) {
    cas 'A':
        afficher "Option A sélectionnée";
        afficher "Traitement spécial A";
        quitter;
    
    cas 'B':
        afficher "Option B sélectionnée";
        afficher "Traitement spécial B";
        quitter;
    
    cas 'C':
        afficher "Option C sélectionnée";
        afficher "Traitement spécial C";
        quitter;
    
    defaut:
        afficher "Option non reconnue";
        afficher "Utilisation de l'option par défaut";
}
```

### **Sélection avec validation**

```zia
fonction evaluerPerformance(code) {
    selon (code) {
        cas 1:
            afficher "Performance excellente";
            afficher "Bonus accordé";
            quitter;
        
        cas 2:
            afficher "Performance satisfaisante";
            afficher "Objectifs atteints";
            quitter;
        
        cas 3:
            afficher "Performance à améliorer";
            afficher "Formation recommandée";
            quitter;
        
        defaut:
            afficher "Code de performance invalide";
            afficher "Veuillez vérifier la saisie";
    }
}

evaluerPerformance(2);
```

---

## 🔄 Boucles : la puissance de la répétition

### **Boucle `pour` : itération contrôlée**

La boucle `pour` est parfaite quand vous savez exactement combien de fois répéter une action :

```zia
// Compte à rebours
afficher "Décompte :";
pour (var i = 10; i >= 1; i = i - 1) {
    afficher i;
}
afficher "Décollage !";
```

```zia
// Calcul de factorielle
var nombre = 5;
var factorielle = 1;

pour (var i = 1; i <= nombre; i = i + 1) {
    factorielle = factorielle * i;
    afficher "Étape ", i, " : ", factorielle;
}

afficher "Factorielle de ", nombre, " = ", factorielle;
```

```zia
// Tables de multiplication
var table = 7;

afficher "Table de multiplication de ", table, " :";
pour (var i = 1; i <= 10; i = i + 1) {
    var resultat = table * i;
    afficher table, " x ", i, " = ", resultat;
}
```

### **Boucle `tantque` : répétition conditionnelle**

La boucle `tantque` continue tant qu'une condition reste vraie :

```zia
// Deviner un nombre
var nombreSecret = 7;
var tentative = 0;
var trouve = faux;

tantque (!trouve et tentative < 5) {
    tentative = tentative + 1;
    var essai = tentative + 4; // Simulation d'une entrée utilisateur
    
    afficher "Tentative ", tentative, " : ", essai;
    
    si (essai == nombreSecret) {
        trouve = vrai;
        afficher "Bravo ! Vous avez trouvé !";
    } sinon si (essai < nombreSecret) {
        afficher "Plus grand !";
    } sinon {
        afficher "Plus petit !";
    }
}

si (!trouve) {
    afficher "Désolé, le nombre était ", nombreSecret;
}
```

```zia
// Calcul de puissance
var base = 2;
var exposant = 8;
var resultat = 1;
var compteur = 0;

tantque (compteur < exposant) {
    resultat = resultat * base;
    compteur = compteur + 1;
    afficher base, " puissance ", compteur, " = ", resultat;
}
```

---

## 🚦 Contrôle de flux avancé : `quitter` et `continuer`

### **Le mot-clé `quitter` : sortir au bon moment**

`quitter` vous permet de sortir prématurément d'une boucle :

```zia
// Recherche d'un élément
var cible = 13;
var trouve = faux;

pour (var nombre = 1; nombre <= 20; nombre = nombre + 1) {
    afficher "Vérification de ", nombre;
    
    si (nombre == cible) {
        afficher "Trouvé ! Le nombre ", cible, " est présent.";
        trouve = vrai;
        quitter; // Sort de la boucle
    }
}

si (!trouve) {
    afficher "Le nombre ", cible, " n'a pas été trouvé.";
}
```

```zia
// Validation avec limite d'erreurs
var erreurs = 0;
var maxErreurs = 3;

pour (var i = 1; i <= 10; i = i + 1) {
    var valeur = i * 2; // Simulation d'une valeur
    
    si (valeur > 15) {
        erreurs = erreurs + 1;
        afficher "Erreur ", erreurs, " : valeur trop grande (", valeur, ")";
        
        si (erreurs >= maxErreurs) {
            afficher "Trop d'erreurs, arrêt du traitement !";
            quitter;
        }
    } sinon {
        afficher "Valeur valide : ", valeur;
    }
}
```

### **Le mot-clé `continuer` : passer à l'étape suivante**

`continuer` ignore le reste de l'itération actuelle et passe à la suivante :

```zia
// Traitement des nombres pairs uniquement
afficher "Nombres pairs de 1 à 20 :";

pour (var i = 1; i <= 20; i = i + 1) {
    si (i % 2 != 0) {
        continuer; // Ignorer les nombres impairs
    }
    
    afficher i, " est pair";
}
```

```zia
// Filtrage avec conditions multiples
pour (var numero = 1; numero <= 15; numero = numero + 1) {
    // Ignorer les multiples de 3
    si (numero % 3 == 0) {
        afficher numero, " est un multiple de 3, ignoré";
        continuer;
    }
    
    // Ignorer les nombres supérieurs à 12
    si (numero > 12) {
        afficher numero, " est trop grand, ignoré";
        continuer;
    }
    
    // Traiter seulement les nombres qui passent les filtres
    afficher "Traitement du nombre : ", numero;
    var double = numero * 2;
    afficher "Son double est : ", double;
}
```

### **Combinaison de `quitter` et `continuer`**

```zia
fonction analyserSequence() {
    var sommesPositives = 0;
    var compteurNegatifs = 0;
    var maxNegatifs = 2;
    
    pour (var valeur = -3; valeur <= 10; valeur = valeur + 1) {
        // Ignorer zéro
        si (valeur == 0) {
            afficher "Zéro ignoré";
            continuer;
        }
        
        // Traiter les nombres négatifs
        si (valeur < 0) {
            compteurNegatifs = compteurNegatifs + 1;
            afficher "Nombre négatif : ", valeur;
            
            si (compteurNegatifs > maxNegatifs) {
                afficher "Trop de nombres négatifs, arrêt !";
                quitter;
            }
            continuer; // Ne pas ajouter à la somme
        }
        
        // Traiter les nombres positifs
        sommesPositives = sommesPositives + valeur;
        afficher "Ajout de ", valeur, " (somme = ", sommesPositives, ")";
    }
    
    afficher "Somme finale des positifs : ", sommesPositives;
    afficher "Nombres négatifs rencontrés : ", compteurNegatifs;
}

analyserSequence();
```

---

## 🎨 Patterns et bonnes pratiques

### **Pattern de validation**

```zia
fonction validerEntree(valeur) {
    si (valeur == nul) {
        afficher "Erreur : valeur nulle";
        retourner faux;
    }
    
    si (valeur < 0) {
        afficher "Erreur : valeur négative";
        retourner faux;
    }
    
    si (valeur > 100) {
        afficher "Erreur : valeur trop grande";
        retourner faux;
    }
    
    retourner vrai;
}

// Utilisation
var donnee = 75;
si (validerEntree(donnee)) {
    afficher "Valeur valide : ", donnee;
} sinon {
    afficher "Valeur rejetée";
}
```

### **Pattern de menu interactif**

```zia
fonction afficherMenu() {
    var choix = 2; // Simulation d'un choix utilisateur
    var continuerProgramme = vrai;
    
    tantque (continuerProgramme) {
        afficher "=== MENU PRINCIPAL ===";
        afficher "1. Nouvelle partie";
        afficher "2. Charger partie";
        afficher "3. Options";
        afficher "4. Quitter";
        
        selon (choix) {
            cas 1:
                afficher "Démarrage d'une nouvelle partie...";
                quitter;
            
            cas 2:
                afficher "Chargement d'une partie existante...";
                quitter;
            
            cas 3:
                afficher "Accès aux options...";
                quitter;
            
            cas 4:
                afficher "Au revoir !";
                continuerProgramme = faux;
                quitter;
            
            defaut:
                afficher "Choix invalide, veuillez réessayer.";
        }
        
        // Simulation de changement de choix
        choix = choix + 1;
        si (choix > 4) {
            choix = 4; // Forcer la sortie
        }
    }
}

afficherMenu();
```

### **Pattern de traitement par lot**

```zia
fonction traiterLot() {
    var traites = 0;
    var erreurs = 0;
    var maxErreurs = 3;
    
    pour (var item = 1; item <= 10; item = item + 1) {
        afficher "Traitement de l'item ", item;
        
        // Simulation d'un traitement qui peut échouer
        var echec = (item % 4 == 0); // Échec tous les 4 items
        
        si (echec) {
            erreurs = erreurs + 1;
            afficher "Échec du traitement de l'item ", item;
            
            si (erreurs >= maxErreurs) {
                afficher "Trop d'erreurs, arrêt du traitement !";
                quitter;
            }
            continuer;
        }
        
        // Traitement réussi
        traites = traites + 1;
        afficher "Item ", item, " traité avec succès";
    }
    
    afficher "Résumé :";
    afficher "- Items traités : ", traites;
    afficher "- Erreurs : ", erreurs;
}

traiterLot();
```

---

## 📏 Règles de style et conventions

### **Indentation et lisibilité**

```zia
// ✅ Bon style
si (condition) {
    // Actions si vrai
    afficher "Condition vraie";
    
    si (autreCondition) {
        // Sous-condition
        afficher "Sous-condition vraie";
    }
} sinon {
    // Actions si faux
    afficher "Condition fausse";
}
```

### **Nommage des variables de contrôle**

```zia
// ✅ Noms expressifs
var estConnecte = vrai;
var nombreTentatives = 0;
var continuerBoucle = vrai;

// ❌ Noms peu clairs
var flag = vrai;
var cnt = 0;
var loop = vrai;
```

### **Éviter les boucles infinies**

```zia
// ✅ Boucle avec garde-fou
var compteur = 0;
var maxIterations = 100;

tantque (condition et compteur < maxIterations) {
    // Traitement
    compteur = compteur + 1;
}

si (compteur >= maxIterations) {
    afficher "Attention : limite d'itérations atteinte";
}
```

---

## 🚀 Vers la maîtrise du contrôle de flux

Les structures de contrôle de Zia vous donnent le pouvoir de créer des programmes sophistiqués qui peuvent :

- **Décider** intelligemment selon les circonstances
- **Répéter** des actions avec précision
- **S'adapter** aux situations imprévues
- **Optimiser** leur exécution

En maîtrisant ces outils, vous transformez vos idées en programmes qui non seulement fonctionnent, mais qui sont aussi élégants, lisibles et maintenables. Chaque structure a sa place et son rôle dans l'orchestre de votre code.

---

*"Un programme bien structuré est comme une symphonie : chaque note (instruction) arrive au bon moment, dans le bon ordre, pour créer une œuvre harmonieuse et fonctionnelle."*