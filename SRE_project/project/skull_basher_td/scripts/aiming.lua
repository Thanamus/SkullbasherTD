function aiming(enemy)
    local aimPos = enemy:getPosition()
    local airTime = getProjectileAirTime()
    local pf = enemy:getPathfinder()
    local nextPos = pf:getNextPoint()
    local dist = distanceXZ(nextPos, aimPos)
    local dir = pf:getDirection()
    local speed = pf:getSpeed();
    local timeToNext = dist/speed;
    local index = pf:getCurrentPathIndex()
    while (timeToNext < airTime) do
        airTime = airTime - timeToNext
        aimPos = nextPos
        index = index - 1
        nextPos = Pathfinder.previewPoint(index)
        dir = vec3.normalize(nextPos - aimPos)
        dist = distanceXZ(nextPos, aimPos)
        timeToNext = dist/speed;
    end
    aimPos = aimPos + airTime*dir*speed
    setAimPos(aimPos)
end

function distanceXZ(a, b)
    return math.sqrt(((a.x - b.x)^2) + ((a.z - b.z) ^ 2))
end