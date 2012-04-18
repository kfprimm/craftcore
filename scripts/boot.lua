
dofile("scripts/loadenv.lua")

log = print
MOD_DIR = "mods/"..arg[1]

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

function import_block(file)
	local attr = load_yaml(file, 'block')
	local block = cc.newBlock()
	for k,v in pairs(attr) do
		if k == "texture" then
			
		end
	end
end

cm = { world = nil }
cm.meta = { name = "", author = "", version = "", website = ""}
cm.textures = { }

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
	end		
end

print("Boot of '"..cm.meta.name.."' complete!")
