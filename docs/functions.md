# Les Fonctions en Zia

## Structurer et réutiliser votre code avec élégance

> *"Une fonction bien conçue est comme un artisan : elle transforme des matières premières (paramètres) en une pièce d'orfèvrerie (résultat), sans gaspillage."*

---

## 🎯 Qu'est-ce qu'une fonction ?

Les fonctions sont le cœur de la programmation en Zia. Elles permettent de créer des programmes modulaires, réutilisables et expressifs, en suivant la logique naturelle de la langue française.

**Pourquoi utiliser des fonctions ?**

- **Réutilisabilité** : Écrivez une fois, utilisez partout
- **Organisation** : Structurez votre code en blocs logiques
- **Maintenance** : Modifiez facilement votre code
- **Lisibilité** : Rendez votre code plus compréhensible

```mermaid
graph LR
    A[Code répétitif] --> B[Fonction]
    B --> C[Code réutilisable]
    D[Problème complexe] --> E[Plusieurs fonctions]
    E --> F[Solution modulaire]
```

---

## 🏗️ Anatomie d'une fonction

```mermaid
flowchart TD
    A[fonction nom] --> B[Paramètres d'entrée]
    B --> C[Corps de la fonction]
    C --> D[Traitement des données]
    D --> E[Valeur de retour]
    
    style A fill:#e1f5fe
    style E fill:#c8e6c9
```

**Structure complète d'une fonction :**

```zia
fonction nomDeLaFonction(parametre1, parametre2) {
    // Variables locales
    var resultat = parametre1 + parametre2;
    
    // Traitement
    si (resultat > 0) {
        afficher "Résultat positif !";
    }
    
    // Retour
    retourner resultat;
}
```

### Fonction la plus simple

```zia
fonction saluer() {
    afficher "Bienvenue dans Zia !";
}

// Appel de la fonction
saluer(); // Sortie: Bienvenue dans Zia !
```

---

## 🔄 Cycle de vie d'une fonction

```mermaid
sequenceDiagram
    participant C as Code appelant
    participant F as Fonction
    participant M as Mémoire
    
    C->>F: 1. Appel avec arguments
    F->>M: 2. Création environnement local
    F->>F: 3. Exécution du corps
    F->>M: 4. Libération mémoire locale
    F->>C: 5. Retour du résultat
```

**Exemple pratique :**

```zia
fonction multiplier(a, b) {
    var resultat = a * b;      // 2. Variables locales créées
    afficher "Calcul en cours..."; // 3. Exécution
    retourner resultat;        // 4. Retour du résultat
}                              // 5. Nettoyage automatique

var produit = multiplier(5, 3); // 1. Appel
afficher produit; // 15
```

---

## 🔐 Portée des variables et environnements

```mermaid
flowchart TB
    subgraph Global["🌍 Environnement Global"]
        A["var globale = 'accessible partout'"]
        
        subgraph Func1["🏠 Environnement Fonction 1"]
            B["var locale1 = 'visible ici'"]
            C["parametre1"]
            
            subgraph Nested["🏡 Fonction Imbriquée"]
                D["var locale2 = 'très locale'"]
                E["parametre2"]
            end
        end
    end
    
    A -.-> B
    B -.-> D
    C -.-> D
    
    style A fill:#ffcdd2,stroke:#d32f2f,stroke-width:2px
    style B fill:#fff3e0,stroke:#f57c00,stroke-width:2px
    style D fill:#e8f5e8,stroke:#388e3c,stroke-width:2px
    style C fill:#e3f2fd,stroke:#1976d2,stroke-width:2px
    style E fill:#f3e5f5,stroke:#7b1fa2,stroke-width:2px
```

**Exemple concret :**

```zia
var messageBienvenue = "Bonjour"; // Variable globale

fonction creerGreeter(nom) {
    var prefixe = "M./Mme "; // Variable de la fonction externe

    fonction saluerFormellement() {
        var ponctuation = " !"; // Variable de la fonction interne
        // Accès à toutes les variables des niveaux supérieurs
        afficher messageBienvenue + " " + prefixe + nom + ponctuation;
    }

    retourner saluerFormellement;
}

var greeter = creerGreeter("Alice");
greeter(); // "Bonjour M./Mme Alice !"
```

---

## 🧩 Closures : La puissance des fonctions

Les closures permettent aux fonctions de "capturer" et de conserver l'accès aux variables de leur environnement de création.

```mermaid
flowchart LR
    A[Fonction externe] --> B[Crée environnement]
    B --> C[Définit fonction interne]
    C --> D[Fonction interne capture l'environnement]
    D --> E[Retourne fonction interne]
    E --> F[Environnement conservé même après fin de fonction externe]
    
    style D fill:#c8e6c9
    style F fill:#ffecb3
```

### Exemple : Générateur de fonctions personnalisées

```zia
fonction creerCompteur(valeurInitiale) {
    // Cette variable est "capturée" par la closure
    var compteur = valeurInitiale;

    fonction incrementer() {
        // La fonction interne modifie la variable de la fonction externe
        compteur = compteur + 1;
        afficher "\nCompteur : ", compteur;
        retourner compteur;
    }

    retourner incrementer; // Retourne la fonction avec son environnement
}

// Création de compteurs indépendants
var compteur1 = creerCompteur(0);    // Commence à 0
var compteur2 = creerCompteur(100);  // Commence à 100

// Chaque compteur garde son propre état
compteur1(); // "Compteur : 1" 
compteur1(); // "Compteur : 2"
compteur1(); // "Compteur : 3"

compteur2(); // "Compteur : 101"
compteur2(); // "Compteur : 102" 

// Les compteurs sont complètement indépendants
afficher "\nPremier compteur à nouveau :";
compteur1(); // "Compteur : 4" (continue où il s'était arrêté)
```

---

## 📊 Comparaison : Avec et sans fonctions

```mermaid
graph TB
    subgraph "Sans fonctions"
        A1[Code répétitif]
        A2[Difficile à maintenir]
        A3[Erreurs fréquentes]
        A4[Code long et confus]
    end
    
    subgraph "Avec fonctions"
        B1[Code réutilisable]
        B2[Maintenance facile]
        B3[Moins d'erreurs]
        B4[Code clair et organisé]
    end
    
    A1 -.->|Refactoring| B1
    A2 -.->|Refactoring| B2
    A3 -.->|Refactoring| B3
    A4 -.->|Refactoring| B4
    
    style A1 fill:#ffcdd2
    style A2 fill:#ffcdd2
    style A3 fill:#ffcdd2
    style A4 fill:#ffcdd2
    style B1 fill:#c8e6c9
    style B2 fill:#c8e6c9
    style B3 fill:#c8e6c9
    style B4 fill:#c8e6c9
```

---

## 🚀 Exercices pratiques

### Exercice 1 : Créer un système de notes
```zia
// Créez une fonction qui gère les notes d'un étudiant
fonction gestionnaireNotes() {
    // À compléter...
}
```

### Exercice 2 : Générateur de mots de passe
```zia
// Créez une fonction qui génère des mots de passe sécurisés
fonction genererMotDePasse(longueur, inclureSymboles) {
    // À compléter...
}
```

### Exercice 3 : Cache de fonctions
```zia
// Créez une fonction qui met en cache les résultats d'autres fonctions
fonction memoiser(fonction) {
    // À compléter...
}
```

---

## 🎓 Points clés à retenir

**Les fonctions en Zia vous permettent de :**

1. **Organiser** votre code en blocs logiques et réutilisables
2. **Encapsuler** la logique complexe derrière une interface simple
3. **Créer** des abstractions puissantes avec les closures
4. **Éviter** la duplication de code et les erreurs
5. **Faciliter** la maintenance et l'évolution de vos programmes

```mermaid
mindmap
  root((Fonctions en Zia))
    Organisation
      Modularité
      Réutilisabilité
      Lisibilité
    Concepts avancés
      Closures
      Fonctions d'ordre supérieur
    Bonnes pratiques
      Validation
      Gestion d'erreurs
      Nommage clair

```

---

> *"Maîtriser les fonctions en Zia, c'est transformer votre façon de programmer : d'un assemblage de code à l'architecture d'une solution élégante et durable."*

**Prochaines étapes :** Explorez les modules et les classes pour structurer encore davantage vos applications Zia !