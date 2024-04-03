# README Projet informatique SEI SICOM 2022-23

## Contenue :

Sur ce git vous trouverez nos programmes classés par dossier :

- include : contient tous nos fichiers headers.
- prog : contient les deux fonctions principales de notre projet.
- src : contient le code source utile à nos programmes.
- tests : contient une partie de nos tests effectués.

## Livrable 1 :

### Objectifs :

Ce projet a plusieurs buts. Dans un premier temps, une fonction réalise la lecture d'une expression régulière (Parsing). Si l'expression régulière fournie est erronée, nous avons détaillé au plus possible l'erreur en question. Sinon, la fonction explicite sa lecture. Puis une seconde fonction vérifie si une chaîne de caractère contient dans ses premiers caractères un "mot" qui vérifie l'expression régulière. 


### Fonctionnement :

Pour le parsing :


La fonction regexp-read.c permet d'effectuer le parsing d'une expression régulière. Son exécution se fait comme suit :

```bash
./bin/prog/regexp-read.exe 'expression_régulière'
 ```

Pour le matching :


La fonction regexp-match.c permet d'effectuer le matching d'une expression régulière. Son exécution se fait comme suit :
   ```bash
./bin/prog/matching.exe 'expression_régulière' 'chaîne_de_caractère
   ```


## Livrable 2 :

### Objectifs :

Le Livrable 2 de notre projet s'inscrit dans la continuité du Livrable 1. L'objectif principal est d'étendre les fonctionnalités de notre programme pour inclure la lecture d'un fichier source contenant des expressions régulières, ainsi que d'un fichier source (.py). On pourra alors réaliser l'analyse lexical du fichier source en cherchant les correspondances avec les expressions régulières données dans le fichier text.


### Fonctionnement :

Pour utiliser notre programme dans le Livrable 2 :

1. **Lecture des fichiers source et d'expressions régulières** :

   La fonction `read_fic` nous permet de transformer les fichiers donnés en argument en une chaîne de caractères.

2. **Lecture et correspondance** :

   La fonction principale `lex` prend comme paramètresun pointeur sur le début de chaîne de caractères représentant les expressions régulières (provenant de `regexp_file.txt`) et un autre pointeur sur le début de chaîne de caractères représentant le fichier source (`.pys`). Elle effectue le matching entre les expressions regulières trouvées dans le fichier et notre code source. 

3. **Exécution** :

   Pour exécuter le programme, utilisez la commande suivante :
   
   ```bash
   ./bin/lexer.exe 'regexp_file.txt' 'test1.pys'
   ```

   Cette commande permettra de lire les expressions régulières depuis le fichier `regexp_file.txt`, d'analyser le fichier source `test1.pys`, et d'afficher les résultats.

   De plus, il est possible de masquer les les expressions regulières "inutile" suivante : blancks, comments et newline. Pour cela il suffit d'ajouter en argument --no-`ce_que_vous_ne_voulez_pas`. Il est biensûr possible de tout supprimer en même temps et l'ordre n'a pas d'importance.

Exemples d'éxecution :

   ```bash
   ./bin/lexer.exe 'regexp_file.txt' 'test.pys' --no-blanks
   ```

  ```bash
   ./bin/lexer.exe 'regexp_file.txt' 'test.pys' --no-comments --no-blanks --no-newline
   ```

## Livrable 3

##Objectifs :

L'objectif principal de livrable est d'implémenter la fonction pyobj_t parse(list_t *lexems) qui effectuera l'analyse syntaxique du langage assembleur Python. Elle renverra un objet Python de type code, ou NULL en cas d’erreur de syntaxe, en affichant et localisant cette erreur.

###Fonctionnement :

La fonction parse prend en entrée une liste de lexèmes obtenue grâce au livrable 2. Elle renvoie un objet Python de type code si l'analyse est réussie, sinon elle renvoie NULL en cas d'erreur de syntaxe. Les erreurs sont affichées et localisées pour faciliter le débogage.

###Exemple d'Exécution :

Pour analyser un fichier source assembleur Python, utilisez la commande suivante :

  ```bash
   ./bin/parser.exe regexp_file.txt test.pys
   ```

## Livrable 4

##Objectifs :

Ce dernier livrable finalise le remplissage du pyobj associé à notre code source. Ce livrable permet d'afficher le bytcode ainsi que le lnotab de notre pyobj. De plus il crée un fichier .pyc qui peut être exécuté grâce à la commande suivante :


  ```bash
  python2.7 nom_du_fichier.pyc
   ```

###Fonctionnement :

La fonction pyasm prend en entrée notre pyobj_t qui a été partiellement complété grâce au livrable 3. Cette fonction y ajoute alors le bytecode et le lnotab. Finalement la fonction write permet à partir de notre pyobj_t complet de créer un fichier .pyc qui permettra enfin d'éxecuter le code. 

###Exemple d'Exécution :


  ```bash
   ./bin/pyasm.exe 'regexp_file.txt' 'test.pys'
   ```
