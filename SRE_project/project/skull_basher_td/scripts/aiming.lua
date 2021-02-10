function aiming(enemy)
    local pf = enemy:getPathfinder()
    local index = pf:getCurrentPathIndex()
    local currPos = enemy:getPosition()
    local nextPos = pf:getNextPoint()
    nextPos.y = 0;
    local aimPos = vec3.new(currPos.x, 0, currPos.z)
    local airTime = getProjectileAirTime() + getLaunchTime()
    local dist = math.sqrt(((nextPos.x - aimPos.x)^2) + ((nextPos.z - aimPos.z) ^ 2))
    local dir = pf:getDirection()
    local speed = pf:getMoveSpeed();
    local timeToNext = dist/speed;
    while (timeToNext < airTime) do
        airTime = airTime - timeToNext
        aimPos = nextPos
        index = index - 1
        nextPos = Pathfinder.previewPoint(index)
        nextPos.y = 0;
        dir = vec3.normalize(nextPos - aimPos)
        dist = math.sqrt(((nextPos.x - aimPos.x)^2) + ((nextPos.z - aimPos.z) ^ 2))
        timeToNext = dist/speed;
    end
    aimPos = vec3.new(aimPos.x, 0, aimPos.z) + airTime*dir*speed
    setAimPos(aimPos)
end