# Programmation Orientée Objet
## À venir dans une future version

---

## 🚧 Fonctionnalité en développement

La **Programmation Orientée Objet (POO)** n'est pas encore implémentée dans Zia. Cette fonctionnalité majeure est prévue pour une version future du langage.

---

## 🎯 Ce qui est prévu

Lorsque la POO sera implémentée, Zia supportera :

- **Classes** avec syntaxe française
- **Héritage** et polymorphisme  
- **Encapsulation** (propriétés privées/publiques)
- **Constructeurs** et méthodes
- **Interfaces** et traits

### Exemple de syntaxe prévue

```zia
// Aperçu de la syntaxe future (non fonctionnelle actuellement)
classe Animal {
    prive var nom;
    
    constructeur(nom) {
        ce.nom = nom;
    }
    
    publique fonction parler() {
        // Méthode à implémenter
    }
}

classe Chien herite Animal {
    publique fonction parler() {
        retourner ce.nom + " fait wouf !";
    }
}
```

---

## 📚 En attendant

Pour l'instant, Zia supporte :
- **[Fonctions](functions.md)** - Programmation procédurale
- **[Structures de contrôle](control-flow.md)** - Logique de base
- **[Variables et types](types.md)** - Gestion des données

---

*Cette page sera mise à jour avec la documentation complète lors de l'implémentation de la POO.*