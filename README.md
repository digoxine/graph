/*
	Memmi Sacha /
	Ahmadi Simab Shokoufeh 	
*/

####################################################
			BFS
####################################################
./bfs file
Affiche le nombre de composante connexe maximale 

####################################################
		Diameter
####################################################
Détermination du diamètre
./src/diameter file
Affiche la taille du graph stocké dans le fichier

####################################################
		Triangles
####################################################

Find Triangles
./src/findTriangles file
Affiche le nombre de triangles contenu dans un graph ainsi que le transitivity ratio
####################################################
	toy graph creation
####################################################
Génération de graph jouets
./src/generateNode nb_nodes nb_clusters p q
Avec p probabilité qu'un noeud soit connecté à tous les noeuds de son cluster
et q probabilité  qu'un noeud soit connecté à tous les noeuds d'un cluster différent.
####################################################
	Community detection
####################################################
Label propagation
./src/label_propagation file
Affiche pour chaque noeud son label sur une ligne

jaccard detection community
./src/jaccard file
Affiche pour chaque noeud son label sur une ligne

Louvain detection community (not accurate)
./src/louvain file
Affiche pour chaque noeud son label sur une ligne 


Classification score
./src/classification_score nb_clusters file
Avec file le fichier labels contenant ce que renvoie nos fonctions de détection
Affiche le score de prediction de notre méthode



####################################################
		Page rank
####################################################
./src/pageRank file
Affiche la matrice page rank

####################################################
		core decomposition
####################################################
./src/kcore file
Affiche:
	-max average density du sous graph 
	-le nombre de noeuds du sous graph
	-edge density
	-Le score de density du sous graph maximum son nombre de noeuds
	-le plus haut degree density
	
