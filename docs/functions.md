# Les Fonctions en Zia

## Structurer et réutiliser votre code avec élégance

> *"Une fonction bien conçue est comme un artisan : elle transforme des matières premières (paramètres) en une pièce d’orfèvrerie (résultat), sans gaspillage."*

---

## 🎯 Introduction aux fonctions

Les fonctions permettent à vos programmes de devenir modulaires, réutilisables et expressifs. En Zia, elles sont conçues pour s’écrire et se lire naturellement, en suivant la logique de la langue française.

Elles vous permettent de :

* Structurer votre code comme des blocs indépendants et réutilisables ;
* Encapsuler de la logique pour éviter la duplication ;
* Créer des abstractions puissantes et élégantes.

---

## 🏗️ Anatomie d’une fonction

```
+---------------------------------------+
|  fonction somme(a, b)                 |
|  +-------------------------------+    |
|  | a, b  ⟶  paramètres           |    |
|  +-------------------------------+    |
|  Corps :                          ⟶  |
|      var total = a + b;               |
|      retourner total;                 |
+---------------------------------------+
```

**Syntaxe minimale :**

```zia
fonction nom(param1, param2) {
    // instructions…
    retourner valeur;
}
```

### Fonction sans paramètre

```zia
fonction saluer() {
    afficher "Bienvenue dans Zia !";
}

saluer();
```

---

## 🧠 Comment une fonction fonctionne

```
Appel : tripler(4)
        │
        ▼
+------------------------+
| fonction tripler(x)    |
|   retourner x * 3;     |
+------------------------+
        │
        ▼
Résultat retourné : 12
```

Une fonction est définie, appelée avec des arguments, puis retourne une valeur. Chaque appel crée un environnement temporaire où les variables sont isolées.

---

## ✨ Exemples fondamentaux

### 1. Saluer un utilisateur

```zia
fonction direBonjour(nom) {
    afficher "Bonjour " + nom + " !";
}

direBonjour("Alexandre");
```

### 2. Calcul d’intérêt composé

```zia
fonction calculerCapital(initial, taux, annees) {
    var capital = initial;
    pour (var i = 1; i <= annees; i = i + 1) {
        capital = capital * (1 + taux / 100);
    }
    retourner capital;
}

var futur = calculerCapital(1000, 5, 3);
afficher "Capital futur : " + futur;
```

### 3. Évaluation rapide

```zia
fonction evalue(score) {
    si (score >= 90) {
        retourner "Excellent";
    } sinon si (score >= 60) {
        retourner "Bon";
    } sinon {
        retourner "À améliorer";
    }
}

afficher evalue(75);
```

---

## 🔐 Portée des variables

```zia
var global = "visible partout";

fonction demoPortee() {
    var local = "visible ici";
    afficher local;       // ok
    afficher global;      // ok
}

demoPortee();
afficher global;          // ok
```

Les variables déclarées à l’intérieur d’une fonction ne sont visibles que dans celle-ci. Les variables globales restent accessibles, sauf si une variable locale les masque temporairement.

---

## 🎭 Closures : Fonctions qui capturent l'état

```
création : creerCompteur(0)
            │
            ▼
+------------------------------+
| compteur = 0                |
| fonction incrementer()      |
|   compteur = compteur + 1;  |
|   retourner compteur;       |
+------------------------------+
            │
            ▼
appel : c1() → 1, puis c1() → 2
```

```zia
fonction creerCompteur(depart) {
    var valeur = depart;
    fonction incrementer() {
        valeur = valeur + 1;
        retourner valeur;
    }
    retourner incrementer;
}

var c1 = creerCompteur(0);
afficher c1(); // 1
afficher c1(); // 2
```

Les fonctions internes gardent en mémoire les variables définies dans leur environnement d'origine. C’est le pouvoir des **closures**.

---

## 🧩 Fonctions imbriquées

```zia
fonction moduleAuthentification() {
    var tentativesRestantes = 3;

    fonction verifier(motDePasse) {
        si (motDePasse == "secret" et tentativesRestantes > 0) {
            afficher "Accès autorisé";
            retourner vrai;
        } sinon {
            tentativesRestantes = tentativesRestantes - 1;
            afficher "Accès refusé. Il reste " + tentativesRestantes + " tentative(s).";
            retourner faux;
        }
    }

    retourner verifier;
}

var authentifier = moduleAuthentification();
authentifier("123");   // refus
authentifier("secret"); // succès
```

Les fonctions peuvent être définies dans d'autres fonctions pour former des **modules privés** ou restreindre l’accès à certaines données.

---

## 🛠️ Bonnes pratiques

1. **Un nom clair** décrivant l’action : `calculerSomme`, `validerMotDePasse`
2. **Une seule responsabilité** par fonction
3. **Commentaires concis** lorsque l’intention n’est pas évidente
4. **Validation** des arguments en entrée
5. **Retour explicite** des résultats

```zia
fonction validerNom(nom) {
    si (nom == nul ou nom.length == 0) {
        afficher "Erreur : nom vide";
        retourner faux;
    }
    retourner vrai;
}
```

---

> *"Une fonction parfaite produit la bonne note, au bon moment, avec la beauté d’une simplicité maîtrisée."*
