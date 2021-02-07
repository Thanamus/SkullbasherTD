function aiming()
    local towerTR =  getTransform(getGameObject());
    local currPos = getTransform(getTarget()):getGlobalPosition()
    towerTR:lookAt(vec3(currPos.x, towerTR:getGlobalPosition().y, currPos.z))
--    local nextPoint = target:getNextPoint()
--    local dist = math.sqrt(math.pow(nextPoint.x - currPos.x, 2) + math.pow(nextPoint.z - currPos.z, 2))
--    local speed = target:getSpeed();
--    local timeToNext = dist/speed;
--    local aimPos = currPos;
--    if (timeToNext > getTravelTime()) then
--        leftover = getTravelTime() - timeToNext
--        nextNextPoint = target:getNextNextPoint();
--        nextDir = normalize(nextNextPoint - nextPoint)
--        aimPos = nextPoint + nextDir*vel*leftover
--    else
--    end
end