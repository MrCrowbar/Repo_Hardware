# Repo_Hardware
Repositorio de los dioses de los fierros del semestre i

¡¡¡¡¡¡¡¡¡CULO EL QUE NO COMENTE SU CODIGO!!!!!!!!!!!!!!!

Cuando vean <algo> quiere decir que deben escribir lo que quieran pero sin los "<>"

Ya que tengan GIT instalado en su computadora se van a algún directorio y hacen lo siguiente:

    git clone <URL DEL REPO>

Una vez que hayan clonado el repo a su compu hagan lo siguiente:

    git pull

    git branch <suNombre>

    git checkout <suNombre>

    git branch --set-upstream-to=origin/<suNombre>

Esto permite que ahora tengan una "branch" propia y ahí pueden modificar todo lo que quieran sin
afectar al archivo original ni el de los demás. También tienen un respaldo en línea al haber hecho
lo de "--set-upstream-to-origin/<suNombre>".
Véanlo como una especie de contenedor en donde pueden agregar archivos nuevos o modificar los que 
ya están pero sin que nada afecte a otras áreas.


Una vez que hayan hecho cambios y los quieran pushear a su branch van a hacer lo siguiente:

    git add . --> Esto permite agregar todos los cambios de los archivos.

    git add <archivo.xxx> --> Esto permite agregar los cambios en los archivos que quieran uno por uno.

    git commit -m "Mensaje corto que explique los cambios que hicieron" --> Traten de ser concizos.

    git push --> Esto sube los cambios a su branch en la nube y aparte a su "branch" local.


Siempre antes de empezar a trabajar en su branch hagan lo siguiente:

    git status -> Les va a decir si hay cambios que faltan subir,etc.

    git pull -> Esto es para "jalar" las modificaciones nuevas que hayan habido en su branch.


En resumen esto es lo que deben hacer antes y después de trabajar(no siempre pero es lo usual).

    git status

    git pull

    git add . // git add archivo

    git commit -m "Mensaje corto que explique los cambios que hicieron"

    git push
