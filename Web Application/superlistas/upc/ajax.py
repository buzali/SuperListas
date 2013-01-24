# coding=ISO-8859-1

from django.utils import simplejson
from dajaxice.decorators import dajaxice_register
from models import *
from django.core import serializers

@dajaxice_register
def prueba(request):
    producto = Producto.objects.all()[0]
    return simplejson.dumps({
        'msg':producto.nombre,
    })
    
@dajaxice_register
def modificar_cantidad(request, idlista, upc, val):
    seleccionado = Seleccionado.objects.get(lista__id=idlista,producto__upc=upc)
    seleccionado.cantidad += val
    seleccionado.save()
    return simplejson.dumps({
        'upc':upc,
        'cantidad':seleccionado.cantidad
    })
    
@dajaxice_register
def borrar_seleccionado(request, idlista, upc):
    seleccionado = Seleccionado.objects.get(lista__id=idlista,producto__upc=upc)
    seleccionado.delete()
    
    sepudo = 'si'
    if seleccionado.id:
        sepudo = 'no'
    return simplejson.dumps({
        'upc': upc,
        'sepudo': sepudo
    })
    
@dajaxice_register
def agregar_producto(request, key, upc):  
    try:
        usuario = Usuario.objects.get(key=key)
    except:
        error = 'Key invalida'
    lista = Lista.objects.get(usuario=usuario)
    superamaupc = clean_upc(upc)
    prod = Producto.objects.filter(upc__contains=superamaupc)
    

    if prod:
        nombre = prod[0].nombre
        fuente = 'superama.com'
    else:
        try:
            nombre = upcdatabase.lookup(upc)
            #name = upcdatabase.query(upc[:-1])
            if type(name) is str:
                prod = Producto.objects.get_or_create(nombre=name, UPC=upc, fam=Familia.objects.get_or_create(nombre='UPCDATABASE')[0])
                fuente = 'upcdatabase.com'
        except:
            error = 'No se puede comunicar con UPCDATABASE'
    
    if prod: # Si se encuentra el producto
        seleccionado, creado = Seleccionado.objects.get_or_create(lista=lista, producto=prod);
        if not creado:
            seleccionado.cantidad += 1
        seleccionado.save()
    else:
        error = 'El producto no existe'
    return simplejson.dumps({
        'nombre':nombre,
        'fuente': fuente,
        'error':error
    })

@dajaxice_register
def refrescar_lista(request, idlista):
    seleccionados = Seleccionado.objects.filter(lista__id=idlista,desplegado=False)
    data = serializers.serialize('json', seleccionados, ensure_ascii=False, use_natural_keys=True)
    seleccionados.update(desplegado=True)
    return data
    
