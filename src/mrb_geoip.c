/*
** mrb_geoip.c - GeoIP class
**
** Copyright (c) MATSUMOTO Ryosuke 2014
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mrb_geoip.h"
#include <GeoIP.h>
#include <GeoIPCity.h>

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  GeoIP *gi;
  GeoIP *isp_gi;
  GeoIPRecord *gir;
  char *city_db;
  char *host;
  char *(*org_function)(GeoIP *gi, const char *host);
} mrb_geoip_data;

static const char *MK_STR(const char *str){
    return str ? str : "N/A";
}

static void mrb_geoip_free(mrb_state *mrb, void *p)
{
  mrb_geoip_data *data = p;

  if (!p) { return; }

  if (data->gir != NULL)
    GeoIPRecord_delete(data->gir);
  if (data->gi != NULL)
    GeoIP_delete(data->gi);
  if (data->isp_gi != NULL)
    GeoIP_delete(data->isp_gi);

  mrb_free(mrb, data);
}

static mrb_value mrb_geoip_close(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_free(mrb, DATA_PTR(self));
  DATA_PTR(self) = NULL;

  return mrb_nil_value();
}

static const struct mrb_data_type mrb_geoip_data_type = {
  "mrb_geoip_data", mrb_geoip_free,
};

static mrb_value mrb_geoip_init(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data;
  char *city_db;
  char *isp_db;
  int argc;

  data = (mrb_geoip_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_geoip_data_type;
  DATA_PTR(self) = NULL;

  argc = mrb_get_args(mrb, "z|z", &city_db, &isp_db);
  data = (mrb_geoip_data *)mrb_malloc(mrb, sizeof(mrb_geoip_data));
  DATA_PTR(self) = data;
  data->city_db = city_db;
  data->host = NULL;
  data->isp_gi = NULL;

  data->gi = GeoIP_open(city_db, GEOIP_INDEX_CACHE);
  if (data->gi == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "GeoIP database open fail");
  }

  if (argc == 2) {
    data->isp_gi = GeoIP_open(isp_db, GEOIP_INDEX_CACHE);
    if (data->isp_gi == NULL) {
      mrb_raise(mrb, E_RUNTIME_ERROR, "GeoIP ISP database open fail");
    }
  }

  data->gir = NULL;

  return self;
}

static mrb_value mrb_geoip_record_by_name(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  char *host;

  if (data->gir != NULL) {
    GeoIPRecord_delete(data->gir);
    data->gir = NULL;
  }
  mrb_get_args(mrb, "z", &host);
  data->host = host;
  data->gir = GeoIP_record_by_name(data->gi, data->host);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record for hostanme");
  }

  data->org_function = GeoIP_org_by_name;
  return self;
}

static mrb_value mrb_geoip_record_by_addr(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  char *addr;

  if (data->gir != NULL) {
    GeoIPRecord_delete(data->gir);
    data->gir = NULL;
  }
  mrb_get_args(mrb, "z", &addr);
  data->host = addr;
  data->gir = GeoIP_record_by_addr(data->gi, data->host);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record for IP address");
  }

  data->org_function = GeoIP_org_by_addr;
  return self;
}

static mrb_value mrb_geoip_country_code(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(data->gir->country_code));
}

static mrb_value mrb_geoip_region(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(data->gir->region));
}

static mrb_value mrb_geoip_region_name(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(GeoIP_region_name_by_code(
          data->gir->country_code, data->gir->region)));
}

static mrb_value mrb_geoip_city(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(data->gir->city));
}

static mrb_value mrb_geoip_postal_code(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(data->gir->postal_code));
}

static mrb_value mrb_geoip_org(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  char *str = data->org_function(data->isp_gi, data->host);
  mrb_value ret = mrb_str_new_cstr(mrb, MK_STR(str));
  free(str);
  return ret;

}

static mrb_value mrb_geoip_latitude(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_float_value(mrb, data->gir->latitude);
}

static mrb_value mrb_geoip_longitude(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_float_value(mrb, data->gir->longitude);
}

static mrb_value mrb_geoip_metro_code(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_fixnum_value(data->gir->metro_code);
}

static mrb_value mrb_geoip_area_code(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_fixnum_value(data->gir->area_code);
}

static mrb_value mrb_geoip_time_zone(mrb_state *mrb, mrb_value self)
{
  mrb_geoip_data *data = DATA_PTR(self);
  if (data->gir == NULL) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "not found GeoIP record,"
       " call record_by_name or record_by_addr method");
  }
  return mrb_str_new_cstr(mrb, MK_STR(GeoIP_time_zone_by_country_and_region(
          data->gir->country_code, data->gir->region)));
}

void mrb_mruby_geoip_gem_init(mrb_state *mrb)
{
    struct RClass *geoip;
    geoip = mrb_define_class(mrb, "GeoIP", mrb->object_class);
    MRB_SET_INSTANCE_TT(geoip, MRB_TT_DATA);
    mrb_define_method(mrb, geoip, "initialize", mrb_geoip_init, MRB_ARGS_ARG(1, 1));
    mrb_define_method(mrb, geoip, "record_by_name", mrb_geoip_record_by_name, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, geoip, "record_by_addr", mrb_geoip_record_by_addr, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, geoip, "country_code", mrb_geoip_country_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "region", mrb_geoip_region, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "region_name", mrb_geoip_region_name, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "city", mrb_geoip_city, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "postal_code", mrb_geoip_postal_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "org", mrb_geoip_org, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "latitude", mrb_geoip_latitude, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "longitude", mrb_geoip_longitude, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "metro_code", mrb_geoip_metro_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "area_code", mrb_geoip_area_code, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "time_zone", mrb_geoip_time_zone, MRB_ARGS_NONE());
    mrb_define_method(mrb, geoip, "close", mrb_geoip_close, MRB_ARGS_NONE());
    DONE;
}

void mrb_mruby_geoip_gem_final(mrb_state *mrb)
{
}

