# coding=ISO-8859-1

from django.http import HttpResponse
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.utils import simplejson
from upc.models import *
from upcdatabase import *
from upcfunctions import *

def agregar(request):
    """api/agregar?key=<userkey>&upc=<upc>&que=<todo|nombre>
        Prueba key: 0481516234200
        prueba upc: 750101047002
        api/agregar?key=0481516234200&upc=750101047002
    """
    nombre = 'n/a'
    fuente = 'n/a'
    error = ''
    if request.method == 'GET':
        key = request.GET.get('key','')
        upc = request.GET.get('upc',0)
        if key:
            try:
                usuario = Usuario.objects.get(key=key)
                lista = Lista.objects.get(usuario=usuario)
                superamaupc = clean_upc(upc) 
                prod = Producto.objects.filter(upc__contains=superamaupc)[0]  # Hacer bien esta query, sin tener que hacer el pinche [0]                   
                if prod:
                    nombre = prod.nombre
                    fuente = 'superama.com'
                else:
                    try:
                        nombre = upcdatabase.lookup(upc)
                        #name = upcdatabase.query(upc[:-1])
                        if type(name) is str:
                            Producto.objects.get_or_create(nombre=name, upc=upc, fam=Familia.objects.get_or_create(nombre='UPCDATABASE')[0])
                            
                            fuente = 'upcdatabase.com'
    
                    except:
                        error = 'No se puede comunicar con upcdatabase.com'
                
                if prod:
                    seleccionado, creado = Seleccionado.objects.get_or_create(lista=lista, producto=prod); # o lista.seleccionado_set.get_or_create(producto=prod)
                    if not creado:
                        seleccionado.cantidad += 1
                    seleccionado.save()
                else:       
                    error = 'El producto no existe'
            except:
                error = 'Key invalida'
        else:
            error = 'No proporcionaste la key'
    que = request.GET.get('que','nombre')
    if que == 'todo':
        output = simplejson.dumps({
            'nombre': nombre,
            'fuente': fuente,
            'error': error
        })
    if que == 'nombre':
        if not error:
            output = nombre
        else:
            output = error

    return HttpResponse(output)