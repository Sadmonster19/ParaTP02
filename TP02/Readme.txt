Readme

Pour installer MPI, aller chercher MSMPI sur le site:
https://msdn.microsoft.com/en-us/library/bb524831(v=vs.85).aspx

Downloadez et runnez le MSI et le exe de la v8.1.
Restart computer.
Pour savoir si vous avez bien, allez dans CMD et runner la commande "set MSMPI".
Si 4 lignes apparaissent vous êtes OK.

Pour exécuter le code de Hello world, allez dans le folder de debug avec la commande et roulez:
mpiexec -n 2 TP02.exe