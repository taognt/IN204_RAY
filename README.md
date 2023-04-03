# RayTracing IN204
---
## Projet
Dans ce projet nous fournissons une bibliothèque C++ pour implémenter des techniques de lancer de rayons.

Nous fournissons entre autres une bibliothèques d'objets permettant de décrire une scène qui sera générée par notre moteur de lancer de rayon.

### Génération du .exe
Pour générer l'exécutable, taper la commande "make all" dans la racine du dossier.

### Génération de l'image et Output
Pour générer l'image, taper la commande "./image_json.exe scene.json"
Les images sont générées dans la racine du dossier.
---
### Description d'une scène :
Une scène est décrite à l'aide d'un fichier json "scene.json" directement à la racine du dossier.
Dans ce fichier, plusieurs objets sont à définir.

- L'image :
Celle-ci est au format 16/9, ces informations sont à renseigner dans le fichier json : La largeur, la profondeur (correspond à la profondeur d'itensité),le ratio et le nombre de rayon à lancer par pixels.

- La camera : 
Il faut définir : sa position, sa focale, sa hauteur.

- Les différentes formes :
Pour toutes les formes on défini un matériau : un matériaux "diffuse" qui diffuse la lumière ou un matériau "métal" qui la réfléchie, et une couleur.
Spheres et plan dans notre projet. Pour les spheres, il faut préciser : son centre, son rayon
Pour les plans : son point origine et sa normale.


### description des classes 
- Dans le dossier include : On trouve tous les headers hpp.

Les classes Camera, Plan, Sphere, Ray, Material, Color et Vector sont assez explicites. A noter que color et vector sont en realité deux mêmes objets.

La classe hittable est une classe virtuelle définissant un objet pouvant être touché par un rayon, la seule méthode utile sera une méthode renvoyant True si le rayon touche l'objet. Un struct data_hit est mis à jour contenant les informations sur l'objet comme le point touché, le matériau, un booléen indiquant s'il s'agit de la face frontale.

A noter que le matériau est un shared_ptr, ce qui facilite l'utilisation des ressources.

La classe List_Hittable définie une liste d'objet, cette liste est nommée "Shapes" dans le main. On cherche alors à savoir si un rayon correspondant à un pixel de l'image touche un des élément de Shapes et si oui, quelles informations le rayon doit en tirer.
