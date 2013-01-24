# coding=ISO-8859-1

from django.http import HttpResponse
from django.shortcuts import render_to_response
from django.template import RequestContext
from django.utils import simplejson
from upc.models import *
import upcdatabase
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
    prod = None
    if request.method == 'GET':
        key = request.GET.get('key','')
        upc = request.GET.get('upc',0)
        if(len(upc) < 4):
            return HttpResponse('UPC muy chiquititititito')
        if key:
            try:
                usuario = Usuario.objects.get(key=key)
                lista = Lista.objects.get(usuario=usuario)
                superamaupc = clean_upc(upc)
                try:
                    prod = Producto.objects.filter(upc__contains=superamaupc)[0]  # Hacer bien esta query, sin tener que hacer el pinche [0]
                    nombre = prod.nombre
                    fuente = 'superama.com'
                except:
                    #El producto no existe, busca en upcdatabase
                    error = 'El producto no existe'
                    try:
                        nombre = upcdatabase.lookup(upc)
                        print nombre
                        #name = upcdatabase.query(upc[:-1])
                        if type(nombre) is str:
                            prod, create = Producto.objects.get_or_create(nombre=nombre, upc=upc, fam=Familia.objects.get_or_create(nombre='d_UPCDATABASE')[0])
                            fuente = 'upcdatabase.com'
                            error = ''
                    except Exception as e:
                        error = 'No se puede comunicar con upcdatabase.com %s' %e                
                if prod:
                    seleccionado, creado = Seleccionado.objects.get_or_create(lista=lista, producto=prod);
                    if not creado:
                        seleccionado.cantidad += 1
                    seleccionado.desplegado = False
                    seleccionado.save()
                    nombre = prod.get_nombre_chido()
            except Exception as e:
                error = 'Key invalida %s' % e
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
      
        
def user(request, username):
    """Desplegar el perfil del usuario y sus listas, si está loggeado, puede editar"""
    return render_to_response('usuario.html', {'username': username})
    
def lista(request, id):
    """Desplegar la lista"""
    lista = Lista.objects.get(id=id)
    seleccionados = lista.seleccionado_set.all()
    seleccionados_familias = lista.seleccionado_set.values_list('producto__fam__descripcion', flat=True).annotate()
    
    seleccionados.update(desplegado=True)
    familias = Familia.objects.all().order_by('nombre')
    return render_to_response('lista.html', {
        'familias':familias,
        'seleccionados_familias': seleccionados_familias,
        'lista': lista,
        'seleccionados': seleccionados
    },context_instance=RequestContext(request))    