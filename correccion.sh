#! /bin/bash

#########    /!\   ¡¡ ATENCIÓN !!   /!\    #########
#   Realiza siempre una COPIA DE SEGURIDAD de tu   #
#    práctica antes de pasarle el autocorrector    #
########################################################################
# Author: Antonio Arias - aam91@alu.ua.es                              #
# Description: Autocorrector prácticas de PED 2010-2011                #
# Version: 1.3 [2012.02.27]                                            #
#                                                                      #
# This program is free software; you can redistribute it and/or modify #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation; either version 2 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# This program is distributed in the hope that it will be useful,      #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with this program; if not, write to the Free Software          #
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,           #
# MA 02110-1301, USA.                                                  #
########################################################################


###
### Configuraciones manuales
###

# Listado de clases (modificar según se avance en la práctica).
practica="buscador" # "tvectorcalendario tpilacalendario tlistacalendario tabbcalendario tavlcalendario"

# Directorio donde se encuentran las carpetas de los tads. Dentro estarán
# los subdirectorios con los tads y las salidas a comparar.
tads="tads"

# Activación de pruebas (ALL por defecto), modificable pasando argumentos al script
test_compilar=1
test_salidas=1
test_valgrind=1

# Archivos a comprimir con la opción -z
dirzip="lib/ include/ src/"




########################################################################
########################################################################



clear
echo "#####################################"
echo "### CORRECTOR DE PRÁCTICAS DE PED ###"
echo "###       By Antonio Arias        ###"
echo "#####################################"
echo;echo

# Directorio de archivos temporales
dir="/tmp/corrector_PED"

# Errores al compilar la práctica
fic_comp="error_compilacion"
err_comp=$dir/$fic_comp
# Errores al compilar los TADs
fic_tads="error_tads"
err_tads=$dir/$fic_tads
# Warning al compilar los TADs
fic_warning="error_warning"
err_warning=$dir/$fic_warning
# Errores de salida al comprobar los TADs
fic_sal="error_salidas"
err_sal=$dir/$fic_sal
# Errores de Valgrind
fic_valgrind="error_valgrind"
err_valgrind=$dir/$fic_valgrind

# Directorio de tads
lstads=""
deflstads=$(ls $tads/)

# Limpieza y compresión (activo por argumentos al script)
test_limpiar=0
text_zip=0


#=== Funciones =========================================================

# Mostrar error al compilar fichero $1 ($2 = fichero de errores)
function ErrorCompilar()
{
    echo -ne "löl"
    echo -ne "========================================================\n" >> $2
    echo -ne "[ /!\\ ERROR - Problemas al compilar \"$1\" ] \n" >> $2
    cat tmp >> $2
    echo -ne "========================================================\n\n" >> $2
}

# Mostrar WARNING al compilar fichero $1 ($2 = fichero de errores)
function WarningCompilar()
{
    echo -ne " (warning!)"
    echo -ne "========================================================\n" >> $2
    echo -ne "[ WARNING! - Warning al compilar \"$1\" ] \n" >> $2
    cat tmp >> $2
    echo -ne "========================================================\n\n" >> $2
}

# Mostrar error al comprobar salidas del TAD $1 ($2 = fichero de errores)
function ErrorSalidas()
{
    echo -ne "!"
    echo -ne "--------------------------------------------------------\n" >> $2
    echo -ne "[ /!\\ ERROR - SALIDAS DISTINTAS \"$1\" ] \n" >> $2
    cat tmp_diff >> $2
    echo -ne "--------------------------------------------------------\n\n" >> $2
}

# Mostrar error al ejecutar VALGRIND en el TAD $1 ($2 = fichero de errores)
function ErrorValgrind()
{
    echo -ne "!"
    echo -ne "--------------------------------------------------------\n" >> $2
    echo -ne "[ /!\\ ERROR - ERROR DE VALGRIND PARA \"$1\" ] \n" >> $2
    cat tmp_val >> $2
    echo -ne "--------------------------------------------------------\n\n" >> $2
}

# Compila, comprueba salidas y valgrind al fichero $2/$1
function Prueba()
{
    # COMPILACIÓN
    g++ -g -std=gnu++0x -Wall $1 $dir/*.o -o $1.exe -I $dir/ 2> tmp
    if test -e $1.exe; then
            echo -n "."

            # SALIDAS
            if [[ $test_salidas = 1 ]]; then
                ./$1.exe > $1.misal 2> tmp_cerr
                diff -b $1.sal $1.misal > tmp_diff
                if test -s tmp_diff; then err_unit=1; ErrorSalidas $2/$1 $err_sal
                else echo -n "."; fi
            fi

            # VALGRIND
            if [[ $test_valgrind = 1 ]]; then
                valgrind --leak-check=full ./$1.exe > tmp_out 2> tmp_val
                grep "All heap blocks were freed -- no leaks are possible" tmp_val > tmp_null
                if (test $? == 1); then
                    err_unit=1
                    ErrorValgrind $2/$1 $err_valgrind
                else
                    echo -n "."
                fi
            fi

    # NO COMPILA
    else err_unit=1; ErrorCompilar $2/$1 $err_tads; fi

    echo -n "  "
    rm -f tmp*
}

# Eliminar ficheros .o
function LimpiarOBJ()
{
    echo -n " == Eliminando ficheros objeto en lib/ ... "
    cd lib/
    rm -f *.o
    cd ..
    echo "OK!"
}

# Limpiar carpeta de los TADS
function LimpiarTADS()
{
    echo -n " == Limpiando carpetas '$tads' ... "
    cd $tads
    for folder in $(ls); do
        cd $folder
        rm -f *.exe
        rm -f *.misal
        cd ..
    done
    cd ..
    echo "OK!"
}

# Limpiar toda la práctica (.o y TADS)
function Limpiar()
{
    LimpiarOBJ
    LimpiarTADS
}

# Comprime la carpeta, limpiándola primero
function Comprimir()
{
    LimpiarOBJ
    echo -n " == Comprimiendo ... "
    rm -f Practica_PED.tgz
    tar czf Practica_PED.tgz $dirzip
    echo "OK!"
}

# Muestra la ayuda del script
function Help()
{
    echo -e "Uso del autocorrector:"
    echo -e "$0 [ -l | -z | [-nc] [-ns] [-nv] ] [\<listado de subdirectorios de TADS>\]"
    echo -e "\t -l"
    echo -e "\t\t Limpiar la carpeta de la práctica de archivos temporales, ejecutables..."
    echo -e "\t -z"
    echo -e "\t\t Comprime toda la carpeta en un archivo .tgz"
    echo -e "\t -nc"
    echo -e "\t\t No Compilar - No compila la práctica (¡asegurarse de que ya lo esté!)"
    echo -e "\t -ns"
    echo -e "\t\t No Salidas - No comprueba las salidas de los tads"
    echo -e "\t -nv"
    echo -e "\t\t No Valgrind - No pasa los tests de memoria de valgrind"
    echo -e "\t <listado de subdirectorios de TADS>"
    echo -e "\t\t Listado de subdirectorios (path relativo al actual) de TADS a comprobar"
    echo -e "\nEjemplo: $0 -nv tads/mistads tads/tcalendario_oficial"
    exit
}


#=== Gestión de argumentos: compilar, usar valgrind, ... ===============

#~ IFS=" -"
for arg in "$@"; do
    case $arg in
        "-l") test_limpiar=1 ;;
        "-z") test_zip=1 ;;
        "-nc") test_compilar=0 ;;
        "-ns") test_salidas=0 ;;
        "-nv") test_valgrind=0 ;;
        "-h") Help ;;
        *) lstads="$lstads $arg" ;;
    esac
done

if [[ $lstads = "" ]]; then lstads=$deflstads; else tads="."; fi;
if [[ $test_zip = 1 ]]; then Comprimir; exit; fi
if [[ $test_limpiar = 1 ]]; then Limpiar; exit; fi

echo "== Opciones de correción =="
echo -n "- Compilar práctica : "; if [[ $test_compilar = 1 ]]; then echo "Sí"; else echo "No"; fi
echo -n "- Comprobar salidas : "; if [[ $test_salidas  = 1 ]]; then echo "Sí"; else echo "No"; fi
echo -n "- Memtest valgrind  : "; if [[ $test_valgrind = 1 ]]; then echo "Sí"; else echo "No"; fi
echo "- Directorios TADS  : "$lstads
echo
read -p "Pulsa ENTER para comenzar la correción (^C para cancelar) . . . "
echo


#=== Inicio de la correción ============================================

rm -f error_*
rm -f -r $dir
mkdir $dir

echo "== COMPROBACIÓN =="

echo -n "> Comprobando directorios: "
if (! test -d lib) || (! test -d include); then
    echo "löl"
    echo "ERROR!! - ¿No existen los directorios lib/ o include/ de la práctica?"
    exit
fi
echo "OK!"

echo -n "> Comprobando ficheros cpp/h: "
boolerr=0
for fic in $practica; do
    if (! test -e lib/$fic.cpp); then
        if (test $boolerr == 0); then echo "löl"; boolerr=1; fi
        echo "/!\\ ERROR - No existe \"$fic.cpp\""
    fi
    if (! test -e include/$fic.h); then
        if (test $boolerr == 0); then echo "löl"; boolerr=1; fi
        echo "/!\\ ERROR - No existe \"$fic.h\""
    fi
done
if (test $boolerr == 1); then exit; fi;

echo "OK!"


#=== Compilación de la práctica ========================================

if [[ $test_compilar = 1 ]]; then

    echo;echo;
    echo "== COMPILACIÓN DE LA PRÁCTICA =="
    echo -n "> Compilando ficheros de lib/: "
    cd lib/
    rm -f *.o

    for fic in $(ls *.cpp | cut -d . -f 1); do
        g++ -g -std=gnu++0x -Wall -c $fic.cpp -I ../include/ 2> tmp

        if [ ! -e $fic.o ]; then ErrorCompilar $fic $err_comp
        else echo -n " . "; fi
    done
    rm -f tmp
    cd ..

    if test -s $err_comp; then
        echo -e "\nERROR - NO COMPILA!"
        echo -e " Errores redirigidos al archivo \"$fic_comp\"\n\n"
        mv $err_comp .
        exit
    fi

    echo " OK!"
fi

cp lib/*.o include/*.h $dir/


#=== Corrección de la práctica (compilar TADS, salidas, valgrind...) ===

echo;echo;
echo -e "== CORRECIÓN ==\n"

cd $tads

for folder in $lstads; do
    cd $folder
    err_unit=0

    echo "> Probando en $tads/$folder/: "
    rm -f *.exe
    rm -f *.misal

    for fic in $( ls *.cpp ); do Prueba $fic $tads/$folder/; done

    if (test $err_unit == 1) then echo "ERROR!"
    else echo "OK!"; fi

    cd ..
done

cd ..


#=== Final de la corrección ============================================

generr=0
for err in $(ls $dir | grep ^error_); do
    echo
    generr=1
    if test $err == $(basename $err_tads); then
        echo -e "/!\\ ERROR - HAY TADS QUE NO COMPILAN!"
        echo -e "Errores redirigidos al archivo \"$fic_tads\""
        mv $err_tads .
    fi
    if test $err == $(basename $err_warning); then
        echo -e "/!\\ WARNING! - AL COMPILAR!"
        echo -e "Avisos de warning redirigidos al archivo \"$fic_warning\""
        mv $err_warning .
    fi
    if test $err == $(basename $err_sal); then
        echo "/!\\ ERROR - LAS SALIDAS NO COINCIDEN!"
        echo "Errores redirigidos al archivo \"$fic_sal\""
        mv $err_sal .
    fi
    if test $err == $(basename $err_valgrind); then
        echo "/!\\ ERROR - ERRORES DE VALGRIND!"
        echo "Errores redirigidos al archivo \"$fic_valgrind\""
        mv $err_valgrind .
    fi
done

if test $generr == 0; then echo -e "\n\n¡ =D ! ¡PRÁCTICA CORRECTA! No hay errores de ningún tipo."; fi

echo;echo
exit
