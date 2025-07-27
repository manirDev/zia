# Projets Débutants
## Apprenez Zia en pratiquant

Cette section présente des projets concrets pour maîtriser les bases de Zia. Chaque projet utilise uniquement les fonctionnalités actuellement disponibles : variables, fonctions, structures de contrôle et sorties.

---

## 🎯 Objectifs pédagogiques

### Ce que vous allez apprendre
- **Variables et types** : Manipulation des nombres, chaînes et booléens
- **Fonctions** : Création et utilisation de fonctions modulaires
- **Structures conditionnelles** : `si`, `sinon si`, `sinon`
- **Boucles** : `pour` et `tantque`
- **Logique de programmation** : Résolution de problèmes étape par étape

### Contraintes actuelles
⚠️ **Note importante** : Ces projets utilisent des valeurs prédéfinies car Zia v1.0 ne supporte pas encore la saisie utilisateur. Les projets se concentrent sur la démonstration des algorithmes et de la logique de programmation.

---

## 📋 Projets disponibles

### 🧮 [Calculatrice](calculatrice.md)
**Niveau** : Débutant  
**Concepts** : Fonctions, conditions, opérations arithmétiques

Créez des fonctions de calcul et démontrez leur utilisation avec différents exemples. Apprenez à structurer votre code et à gérer les cas d'erreur.

**Exemple de sortie :**
```
=== Démonstration Calculatrice ===
15 + 7 = 22
15 - 7 = 8
15 * 7 = 105
15 / 7 = 2.14
Erreur: Division par zéro !
```

---

### 🎲 [Générateur de tables](tables.md)
**Niveau** : Débutant  
**Concepts** : Boucles, fonctions, formatage

Créez un programme qui génère des tables de multiplication et autres séquences mathématiques. Parfait pour maîtriser les boucles et le formatage.

**Exemple de sortie :**
```
=== Table de 7 ===
7 x 1 = 7
7 x 2 = 14
7 x 3 = 21
...
```

---

## 🛠️ Conseils pour réussir

### Structure recommandée
```zia
// 1. Variables globales (si nécessaires)
var donnees_test = 42;

// 2. Fonctions utilitaires
fonction calculer_quelque_chose(a, b) {
    retourner a + b;
}

// 3. Fonctions de démonstration
fonction demonstrer_calculs() {
    afficher "=== Démonstration ===";
    // Votre logique ici
}

// 4. Programme principal
demonstrer_calculs();
```

### Bonnes pratiques
- **Utilisez des données variées** pour tester vos fonctions
- **Documentez vos fonctions** avec des commentaires
- **Testez les cas limites** (zéro, nombres négatifs, etc.)
- **Organisez votre sortie** pour qu'elle soit lisible

---

## 🎓 Après les projets débutants

Une fois ces deux projets maîtrisés, vous serez prêt pour :
**Projets intermédiaires** (quand les modules seront disponibles).

---

## 🚀 Commencer maintenant

Choisissez votre premier projet et découvrez la puissance de Zia !

**[🧮 Calculatrice](calculatrice.md)** • **[🎲 Générateur de tables](tables.md)**