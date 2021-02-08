function shooting(time)
    local turr = getGameObject()
    local turrTR = getTransform(turr)
    turrTR:lookAt(vec3.new(getAimPos().x, turrTR:globalPosition().y, getAimPos().z), vec3.new(0, 1, 0))
    local arm = turr:getChildByName("Arm")
    local animator
    if (arm) then
        animator = getAnimator(arm)
    end
    if(animator and animator:getAnimationState() ~= "launch") then
        animator:setAnimationState("launch")
    else
        if (not animator or animator:getAnimationForState("launch"):hasEnded(time)) then
            local projectile = makeProjectile()
            local projectilePos = getTransform(projectile):globalPosition()
            local direction = vec3.normalize(getAimPos() - projectilePos)
            local distance = vec3.length(getAimPos() - projectilePos)
            local velocity = btVector3.new(direction.x, direction.y, direction.z)
            velocity = velocity * (distance / getProjectileAirTime())
            setVelocity(projectile, velocity)
            setReadyToShoot(false)
            setAimPos(vec3.new(-1, -1, -1))
        end
    end
end