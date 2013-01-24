# coding=ISO-8859-1

import sys
from xmlrpclib import ServerProxy
from BeautifulSoup import BeautifulStoneSoup
from urllib2 import build_opener, HTTPCookieProcessor, HTTPSHandler
from urllib import urlencode
from cookielib import CookieJar
import re

rpc_key = '5f3155f8f9ab81c98fdf1272b9179edfa59dd036'

def query(upc):
    """Saca la descripción del producto usando la API de upcdatabase.com"""
    
    queryurl = 'http://www.upcdatabase.com/xmlrpc'
    s = ServerProxy(queryurl)
    params = { 
        'rpc_key': rpc_key,
        'upc': upc 
    }
    response = s.lookup(params)
    if response.has_key('description'):
        description = response['description']
        description = str(BeautifulStoneSoup(description, convertEntities=BeautifulStoneSoup.HTML_ENTITIES))
        return description
    else:
        return response
        
def lookup(upc):
    """Saca la descripcion del producto usando el lookup de upcdatabase.com"""
    
    queryurl = 'http://www.upcdatabase.com/item.asp' 
    opener = build_opener()
    params = {
        'upc': upc,
    }
    values = urlencode(params) # Sets the parameters as the POST data
    response = opener.open(queryurl, values) # Makes de request with a url and data
    html = response.read()
    exp = r'Description</td><td></td><td>(?P<description>.*?)<'
    e = re.compile(exp)
    
    item = e.search(html)
    if item:
        description = item.group('description')
        description = str(BeautifulStoneSoup(description, convertEntities=BeautifulStoneSoup.HTML_ENTITIES))
        return description
    else:
        print 'UPC: %s buscado en updatabase, no encontrado' % upc
        return 0

def neighbors(upc):
    cj = CookieJar()
    opener = build_opener(HTTPCookieProcessor(cj))
   
    username = 'davoclavo'
    password = 'qwerty'
    
    loginurl = 'http://www.upcdatabase.com/login.asp'
  
    
    # CHECAR EL NOMBRE DE LOS PARAMETROS
    params = {
        'username': username,
        'password': password,
    }
        
    values = urlencode(params)

    response = opener.open(loginurl, values) # Hace log in y guarda la cookie
    html = response.read()

    if html.find('Welcome: ' + username) > -1:
        print '--Authenticated ' + username
    else:
        print '--Wrong username/password'
        return -1

    queryurl = 'http://www.upcdatabase.com/neighbors.asp?upc=%s' % upc
    response = opener.open(queryurl)
    html = response.read()
    
    exp = r'/item/(?P<upc>\d*)">\1.*?<td>(?P<description>.*?)</td>'
    e = re.compile(exp, re.DOTALL)
    
    items = e.finditer(html)
    for item in items:
        upc = item.group('upc')
        description = item.group('description')
        description = str(BeautifulStoneSoup(description, convertEntities=BeautifulStoneSoup.HTML_ENTITIES))
        print upc + ' - ' + description
    
    # BUSCAR Y GUARDAR EN LA DB los items
    

from datetime import datetime
from upc.models import *
    
def agregar_producto_superama(idlista=1,nombre='Lista diario'):
    cj = CookieJar()
    opener = build_opener(HTTPCookieProcessor(cj), HTTPSHandler()) # Creo que no se necesita el HTTPSHandler, pero por si las flais
    opener.addheaders = [('User-Agent', 'Mozlla/5.0 (Macintosh; U; Intel Mac OS X 10.6; en-US; rv:1.9.2.11) Gecko/20101012 Firefox/3.6.11')]
   
    email = 'anaavales@mailinator.com'
    password = '1234qwer'
    
    loginurl = 'https://www.superama.com.mx/validateLogin.asp'
  
    # CHECAR EL NOMBRE DE LOS PARAMETROS
    params = {
        'customerEmail':email,
        'customerPassword':password,
        'Submit':'Ingresar',
        'surl':''
    }
    values = urlencode(params)
    response = opener.open(loginurl, values) # Hace log in y guarda la cookie
    html = response.read()

    if 'error.gif' not in html:
        print '--Boveda superama abierta ' + email
    else:
        print '--Tu password es incorrecto'
    
    tiempo = datetime.now()
    nombre = nombre + ' %s' % tiempo.strftime('%d-%m-%Y') #Agregar fecha al nombre de la lista
    
    
    cookiename = 'simpleCart0'
    
    seleccionados = Seleccionado.objects.filter(lista__id=idlista)
    
    index = 1
    for seleccionado in seleccionados:
        if index > 1:
            galleta += '~~'
        galleta += 'c%s||1||%s||c||%s||-||' % (index, seleccionado.producto.upc, seleccionado.cantidad)
        index += 1   

    queryurl = 'http://www.superama.com.mx/addCartToList.asp'
#    params = {
#        'Upc':upc,
#    }
#    values = urlencode(params)
#    response = opener.open(queryurl, values)
#    html = response.read()
#    
#    fname = 'superama.html'
#    f = open(fname, 'w')
#    f.write(html)
#    f.close()
#    
#    nombrelista= 'prueba'
#    listaurl = 'http://www.superama.com.mx/superama-nuevo/addCartToList.asp'
#    params = {
#        'nombrelista':nombrelista,
#    }
#    
#    
#    nombresavelista= 'probando'
#    savelistaurl = 'http://www.superama.com.mx/superama-nuevo/saveList.asp'
#    params = {
#        'newListName':nombresavelista,
#        'submit':'Crear Lista'
#    }
    
