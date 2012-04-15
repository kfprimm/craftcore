
local bar = cm.textures["gui.item-bar"]
local cursor = cm.textures["icons.cursor"]
local ebar = cm.textures["gui.exp-bar"]

function cm.renderHUD()
	cc.drawImage(bar,(cc.contextWidth() - 182)/2, cc.contextHeight() - 22)
	cc.drawImage(cursor, (cc.contextWidth() - 16)/2, (cc.contextHeight() - 16)/2)
	cc.drawImage(ebar, (cc.contextWidth() - 16)/2, (cc.contextHeight() - 16)/2)
end
