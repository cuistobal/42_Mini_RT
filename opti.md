Voici les principaux axes d’amélioration des performances pour ton miniRT, triés par ordre d’importance/impact :

1. Traversée BVH (ordre et logique)
Optimiser l’ordre de visite des enfants (toujours explorer le plus proche d’abord, early exit si possible).
Utiliser une pile explicite (itératif) pour éviter la récursion profonde et améliorer la localité mémoire.
Limiter le nombre d’objets par feuille pour éviter des tests inutiles.
2. Construction BVH
Utiliser un split SAH efficace (Surface Area Heuristic) : c’est déjà le cas, mais tu pourrais améliorer la rapidité du tri (remplacer le bubble sort par un quicksort ou radix sort).
Aplatir le BVH en tableau pour améliorer la localité mémoire lors de la traversée.
3. Intersections géométriques
Optimiser les fonctions d’intersection (sphere, plane, etc.) pour limiter les calculs inutiles et le branching.
Vectoriser (SIMD) les calculs si possible, surtout pour les AABB.
4. Boucle de rendu
Paralléliser le rendu (OpenMP, pthreads, etc.) pour utiliser tous les cœurs CPU.
Réduire les accès mémoire inutiles (éviter les copies de structures volumineuses).
5. Gestion mémoire
Réduire les allocations dynamiques dans les boucles critiques (pré-allouer les buffers, réutiliser les structures).
Utiliser des structures compactes pour améliorer le cache.
6. Fonctions utilitaires
Supprimer le branching inutile (ex : dans les conversions de couleurs, clamp, etc.).
Remplacer les boucles naïves par des versions optimisées (ex : bubble sort → quicksort).
7. Profilage et micro-optimisations
Profiler régulièrement pour cibler les vrais points chauds.
Optimiser les accès aux objets et aux lumières (ex : tableaux plutôt que listes chaînées).
Résumé (ordre d’impact) :

Traversée BVH (ordre, pile, early exit)
Construction BVH (split, tri, aplatir)
Intersections géométriques (optim, SIMD)
Parallélisation du rendu
Gestion mémoire (allocs, structures)
Fonctions utilitaires (branching, tri)
Profilage ciblé
Commence par les points 1 à 4 pour les plus gros gains, puis affine avec les autres selon les résultats du profilage.
