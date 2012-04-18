
CHUNKSIZE = 16

local octaves = 2
local freq    = 4
local seed    = 80185

chunk = cc.worldAddChunk(cm.world)

for z = 0,CHUNKSIZE - 1 do
	for x = 0,CHUNKSIZE - 1 do
		local amp, vec = 1, { (x/CHUNKSIZE) * freq, (z/CHUNKSIZE) * freq }
		local result = 0
		for i = 0, octaves - 1 do
			result = result + noise2(vec)*amp
			vec[1] = vec[1] * 2
			vec[2] = vec[2] * 2
			amp = amp * 0.5
		end

		result = (result * 2) + 3;
		for y = 0, CHUNKSIZE - 1 do
			if y + 1 < result then
				cc.chunkSetBlock(chunk, 1, x, y, z)
			elseif y < result then
				cc.chunkSetBlock(chunk, 2, x, y, z)
			else
				cc.chunkSetBlock(chunk, 0, x, y, z)
			end
		end
	end
end


