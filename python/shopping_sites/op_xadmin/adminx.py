from .models import ops
import xadmin

# Register your models here.

class opsAdmin(object):
    list_display = ('opera_name',  'opera_age', )
    
xadmin.site.register(ops,  opsAdmin)
