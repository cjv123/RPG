
map = load_data(sprintf("Data/Map%03d.rxdata", 1))
data_tilesets    = load_data("Data/Tilesets.rxdata")
tileset = data_tilesets[map.tileset_id]
tileset_name = tileset.tileset_name
autotile_names = tileset.autotile_names
panorama_name = tileset.panorama_name
panorama_hue = tileset.panorama_hue


tilemap = Tilemap.new(Viewport.new(0,0,640,480))
tilemap.tileset = RPG::Cache.tileset(tileset_name)
for i in 0..6
  autotile_name = autotile_names[i]
  tilemap.autotiles[i] = RPG::Cache.autotile(autotile_name)
end
tilemap.map_data = map.data

loop do
  tilemap.update
  Graphics.update
end
