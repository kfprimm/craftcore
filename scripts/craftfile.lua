
function craftfile()
	local ret = { textures = {} }
	for i,f in ipairs({ "name", "author", "version", "website" }) do _G[f] = function(v) ret[f] = v end ret[f] = "" end
	
	function texture(values)
		atlas = cc.loadAtlas(MOD_DIR.."/"..values[1])
		for i = 2, #values do
			ret.textures[values[i][1]] = cc.atlasImage(atlas, values[i][2], values[i][3], values[i][4], values[i][5])
		end
	end
	
	function block(b)
	
	end
	
	function hud(values)
		ret.hud = {}
		ret.hud.width  = values[1]
		ret.hud.height = values[2]
	end
	
	require("Craftfile")
	
	return ret
end

