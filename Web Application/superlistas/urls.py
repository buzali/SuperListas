from django.conf.urls.defaults import patterns, include, url
from upc.views import *
#form upc.api import *
from django.conf import settings
from django.views.generic.simple import direct_to_template

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

from dajaxice.core import dajaxice_autodiscover
dajaxice_autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'superlistas.views.home', name='home'),
    # url(r'^superlistas/', include('superlistas.foo.urls')),
    url(r'^api/agregar', agregar),
   

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
    url(r'^u/(\w+)$', user),
    url(r'^lista/(\d+)$', lista),
    
    # Dajaxice
    (r'^prueba$', direct_to_template, {'template': 'prueba.html'}),
    (r'^%s/' % settings.DAJAXICE_MEDIA_PREFIX, include('dajaxice.urls')),
)
