
-- Craftcore
-- Mod boot script

info = dofile("mods/"..arg[1].."/Craftfile")

blocks, count = {}, 0
for i, b in ipairs(info.blocks) do
	blocks[b] = dofile("mods/"..arg[1].."/blocks/"..b..".lua")
	cc.addBlock()
	count = count + 1
end

