# Utilisation du programme

Vous pouvez jouer avec les paramètres de la scène dans la fonction ``cornell_box`` de main.cc et décommenter les objets que vous voulez voir dans la scène.

Compilation, exécution et sauvegarde du rendu sous le format .ppm :
```
g++ -O3 main.cc -o main && ./main > image.ppm
```

- rendu_mesh.ppm : Rendu de la boite de cornnell avec import du maillage star.obj : materiau diffus (lambertian) de couleur blanche. 
    - Résolution : 600x600 
    - samples_per_pixel = 500
- rendu_spheres.ppm : Rendu de deux sphères dans la boite de cornnell.
    - Une sphère réfléchissante : materiau metal
    - Une sphère transparente : materiau dielectric avec un indice de réfraction de 1.5
    - Résolution : 600x600 
    - samples_per_pixel = 1000
- rendu_moving_sphere.ppm : Rendu d'une sphère en mouvement (moving_sphere) se déplaçant de (225.0, 150.0, 250.0) à t = 0 jusqu'à (325.0, 150.0, 250.0) à t =1.
    - Résolution : 600x600 
    - samples_per_pixel = 1000
- rendu_blurred_spheres.ppm : Rendu de 3 sphères, focus sur la sphère du milieu.
    - Focale à (position(sphere_milieu) - position(camera)).length()
    - Ouverture (aperture) de f/5
    - Résolution : 600x600 
    - samples_per_pixel = 1000

# Rendu Projet de lancer de rayon
Mon ray tracer est basé sur la série de livres [Ray Tracing in One Weekend](https://raytracing.github.io/) par Peter Shirley et en particulier le [livre 1](https://raytracing.github.io/books/RayTracingInOneWeekend.html) et le [livre 2](https://raytracing.github.io/books/RayTracingTheNextWeek.html) 


## Phase 3 : Chargement et rendu d'un maillage 3D

Je me suis focalisé sur les maillages triangulaire. 

J'ai d'abord crée la classe triangle.h permettant de créer un objet hittable et de stocker les différents attiributs d'un objet triangle

Au lieu de faire un algorithme d'intersection rayon/plan comme implémenté pour les quadrilatères dans la fonction ``quad::hit()``, j'ai décidé d'implémenter l'[algorithme Möller-Trumbore](https://fr.wikipedia.org/wiki/Algorithme_d%27intersection_de_M%C3%B6ller%E2%80%93Trumbore) pour un calcul rapide de l'intersection rayon/triangle.

La fonciton mesh permet de parcourir un fichier .obj et instancie dans la scène autant de triangle que nécessaire   


## Phase Finale
### 1. Sphère Réfléchissante (cf. classe metal dans material.h)

On considère qu'il s'agit d'un miroir parfait. On applique donc uniquement la loi de réflexion spéculaire :
> rayon_réfléchi = rayon_incident - 2 * dot(rayon_réfléchi, rayon_incident) * normale

### 2. Sphère Transparente (cf. classe dielectric dans material.h)

A l'interface entre les deux milieux, au lieu de séparer le rayon en composantes spéculaire et transmise, on choisit de manière aléatoire (selon la réflectance) entre renvoyer le rayon réfléchi ou le rayon réfracté.

Pour la réfraction on applique la loi de Snell-Descartes :
> Soit cos(theta) = dot(-rayon_réfléchi, normale)
>
> et Rperp = 1/ indice_réfraction * (rayon_réfléchi + cos(theta) * normale)
>
> Alors rayon_réfracté = Rperp - sqrt(1- Rperp.length()²) * normale

(cf. [Livre 1 - Chapitre 10](https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics/totalinternalreflection) pour plus de détails)
## Effets et options implémentés

- Profondeur de champ ajustable (cf. ``camera::initialize()`` et ``camera::get_ray()``)
- Flou de mouvement (cf. moving_sphere.h)

# Pistes de travail, axes d'améliorations
- Optimisation CPU : Boucle de rendu (fonction ``scene::render()``) sûrement multithreadable en partitionnant l'image rendue en grilles assignables aux multiples threads.
- Gérer l'import de maillages d'autres format que le .obj (librairie assimp)