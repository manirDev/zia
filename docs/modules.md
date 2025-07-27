# Modules et Bibliothèques
## À venir dans une future version

---

## 🚧 Fonctionnalité en développement

Le **système de modules** et les **bibliothèques** ne sont pas encore implémentés dans Zia. Ces fonctionnalités essentielles sont prévues pour une version future du langage.

---

## 🎯 Ce qui est prévu

### Système de modules (v1.2)
- **Import/Export** avec syntaxe française
- **Gestion des fichiers multiples**
- **Espaces de noms** pour éviter les conflits
- **Modules locaux** et externes

### Bibliothèque standard (v1.2+)
- **Manipulation de fichiers**
- **Gestion du temps et dates**
- **Fonctions mathématiques avancées**
- **Traitement de chaînes**
- **Collections et structures de données**

---

## 💡 Exemples de syntaxe prévue

### Import et export
```zia
// Aperçu de la syntaxe future (non fonctionnelle actuellement)

// Dans mathematiques.zia
exporter fonction calculer_factorielle(n) {
    si (n <= 1) retourner 1;
    retourner n * calculer_factorielle(n - 1);
}

exporter const PI = 3.14159;

// Dans main.zia
importer { calculer_factorielle, PI } depuis "mathematiques";
// ou
importer mathematiques depuis "mathematiques";

var resultat = calculer_factorielle(5);
afficher "Factorielle de 5: ", resultat;
```

### Bibliothèque standard
```zia
// Manipulation de fichiers
importer { lire_fichier, ecrire_fichier } depuis "fichiers";

var contenu = lire_fichier("data.txt");
ecrire_fichier("sortie.txt", "Nouveau contenu");

// Gestion du temps
importer { obtenir_date, formater_date } depuis "temps";

var maintenant = obtenir_date();
var date_formatee = formater_date(maintenant, "jj/mm/aaaa");

// Mathématiques
importer { racine_carree, puissance, aleatoire } depuis "maths";

var x = racine_carree(16);
var y = puissance(2, 8);
var nombre_aleatoire = aleatoire(1, 100);
```

---

## 📚 En attendant

Pour l'instant, vous pouvez organiser votre code avec :
- **[Fonctions](functions.md)** - Modularité de base
- **Fichiers séparés** - Organisation manuelle (un fichier à la fois)
- **Conventions de nommage** - Structuration du code

### Exemple d'organisation actuelle
```zia
// Toutes les fonctions dans un seul fichier pour l'instant
fonction utilitaire_formater_nom(nom) {
    retourner "M./Mme " + nom;
}

fonction calcul_surface_rectangle(l, h) {
    retourner l * h;
}

fonction calcul_surface_cercle(rayon) {
    retourner 3.14159 * rayon * rayon;
}

// Code principal
var nom = utilitaire_formater_nom("Dupont");
var surface = calcul_surface_rectangle(10, 5);
afficher nom, " - Surface: ", surface;
```

---

## 🔄 Mises à jour

Cette page sera entièrement réécrite avec :
- **Documentation complète** des modules
- **Référence de la bibliothèque standard**
- **Tutoriels et exemples pratiques**
- **Guide de création de modules**

---

*Cette page sera mise à jour avec la documentation complète lors de l'implémentation du système de modules.*