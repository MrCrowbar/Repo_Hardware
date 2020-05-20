# Repo_Hardware
Repositorio de los dioses de los fierros del semestre i

¡¡¡¡¡¡¡¡¡CULO EL QUE NO COMENTE SU CODIGO!!!!!!!!!!!!!!!

Una vez que hayan clonado el repo a su compu hagan lo siguiente:

1.- git pull

2.- git branch suNombre

3.- git checkout suNombre

4.- git branch --set-upstream-to=origin/suNombre

Esto permite que ahora tengan una "branch" propia y ahí pueden modificar todo lo que quieran sin
afectar al archivo original ni el de los demás. También tienen un respaldo en línea al haber hecho
lo de "--set-upstream-to-origin/suNombre".
Véanlo como una especie de contenedor en donde pueden agregar archivos nuevos o modificar los que 
ya están pero sin que nada afecte a otras áreas.


Una vez que hayan hecho cambios y los quieran pushear a su branch van a hacer lo siguiente:

1.-    git add . --> Esto permite agregar todos los cambios de los archivos.

    git add archivo.xxx --> Esto permite agregar los cambios en los archivos que quieran uno por uno.

2.- git commit -m "Mensaje corto que explique los cambios que hicieron" --> Traten de ser concizos.

3.- git push --> Esto sube los cambios a su branch en la nube y aparte a su "branch" local.


Siempre antes de empezar a trabajar en su branch hagan lo siguiente:

1.- git status -> Les va a decir si hay cambios que faltan subir,etc.

2.- git pull -> Esto es para "jalar" las modificaciones nuevas que hayan habido en su branch.


En resumen esto es lo que deben hacer antes y después de trabajar(no siempre pero es lo usual).

1.- git status

2.- git pull

3.- git add . // git add archivo

4.- git commit -m "Mensaje corto que explique los cambios que hicieron"

5.- git push
