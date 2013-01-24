from urllib2 import urlopen, quote
from upc.models import *
import re, json

def scrape_familia():
    """Saca nombres de familia"""
    url = "http://www.superama.com.mx/superama-nuevo/home.asp" #http://www.superama.com.mx/home.asp
    exp = r'"(d_[_\w%, ]*)"'
    e = re.compile(exp)
    response = urlopen(url)
    html = response.read()

#     
#     fname = 'superama.html'
#     f = open(fname, 'w')
#     f.write(html)
#     f.close()
    
    for a in e.findall(html):
        print '--Added Familia: ' + quote(a)
        Familia.objects.get_or_create(nombre=quote(a))

        

def scrape_productos():
    url_base = "http://www.superama.com.mx/createXML.asp?typeSearch=compra-por-departamento&departmentName=%s"
    for fam in Familia.objects.filter(nombre__contains='_d'): # El d_ para que solo cheque los productos de superama
        print fam
        url = url_base %fam.nombre
        html = urlopen(url).read()
        prod_list = json.loads(html, encoding="ISO-8859-1", strict=False)
        for p in prod_list:
            Producto.objects.get_or_create(nombre=p['name'], UPC=p['up'], precio=p['d'], fam=fam)
            print p['name']