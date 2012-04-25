
log = print
MOD_DIR = "mods/"..arg[1]

package.path = package.path..";"..MOD_DIR.."/?.lua;"..MOD_DIR.."/?"

require("scripts/loadenv")

function load_yaml(file, tag)
	input = io.open(MOD_DIR.."/"..file)
	text = input:read("*all")
	input:close()

	info = yaml.load(text)
	
	mod = info[tag]
	assert( mod ~= nil )
	return mod
end

function import_atlas(file)
	log("Loading textures from '"..file.."'")
	local attr = load_yaml(file, 'atlas')
	local atlas = cc.loadAtlas(MOD_DIR.."/"..attr['url'])
	for i,sub in ipairs(attr['sub']) do
		local name = sub.name

		if attr['name'] ~= nil then name = attr['name'].."."..name end
	
		cm.textures[name] = cc.atlasTexture(atlas, sub.rect[1], sub.rect[2], sub.rect[3], sub.rect[4])
		log("Loaded '"..name.."'")
	end
end

function import_side(block, side, attr)
	for k,v in pairs(attr) do
		if k == "texture" then
			cc.blockSetTexture(block, side, cm.textures[attr.texture])
		elseif k == "color" then
			cc.blockSetColor(block, side, attr.color[1]/255, attr.color[2]/255, attr.color[3]/255)
		end
	end
end

function import_block(file)
	local attr = load_yaml(file, 'block')
	log("Importing '"..attr['name'].."'")
	local block = cc.newBlock()
	for k,v in pairs(attr) do
		if k == "texture" then
			cc.blockSetTexture(block, -1, cm.textures[v])
		elseif k == "name" then
			cc.blockSetName(block, v)
		elseif k == "top" then
			import_side(block, CHUNK_TOP, v)
		elseif k == "bottom" then
			import_side(block, CHUNK_BOTTOM, v)
		end
	end
	cc.worldAddBlock(cm.world, block)
end

local attr = load_yaml('Craftfile.yml', 'craft_mod')

for k,v in pairs(attr) do
	if k == "meta" then
		for i,d in pairs(v) do
			cm.meta[i] = d
		end
	elseif k == "atlases" then
		for i,d in ipairs(v) do
			import_atlas(d)
		end
	elseif k == "blocks" then
		for i,d in ipairs(v) do
			import_block(d)
		end
	elseif k == "hud" then
		for n,f in pairs(v) do
			if n == "draw" then
				cm.renderHUD = loadfile(MOD_DIR.."/"..f)
			end
		end
	end		
end

print("Boot of '"..cm.meta.name.."' complete!")
