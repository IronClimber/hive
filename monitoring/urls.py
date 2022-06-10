from django.urls import path

from . import views

urlpatterns = [
    path('',views.index, name = 'index'),
    path('publish/',views.publish, name = 'publish'),
    path('duration/',views.get_duration, name = 'duration'),
    path('period/',views.get_period, name = 'period'),
    path("duration/<int:value>", views.set_duration, name="duration_value"),
    path("period/<int:value>", views.set_period, name="period_value"),
    path("csv/", views.get_csv, name="csv"),
    path("last/<int:value>", views.get_last_objects, name="last_objects")
]
