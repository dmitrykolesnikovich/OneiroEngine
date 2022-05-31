-- creates characters, sprites, and other means
me = Character("Me")
-- second parameter is aspect ratio for sprite
-- for normal resizing sprite to window size you need keep aspect ratio
local sprite = Sprite2D("Assets/Images/sprite.png", true)
-- but in background you don't need to enabling aspect ratio
local background = Sprite2D("Assets/Images/background.jpg", false)
local music = Music("Assets/Audio/music.mp3")

-- you must to set the var name that was passed to the label ctor
-- because for call to label function engine need know name of variable
start = Label("start")
-- entry point in label is main function
-- you can use colon when you call or define function
function start:main()
    background:show()
    sprite:show()
    -- you can move sprite (x, y, z)
    sprite:move(0.25, 0.0, 0.0);
    music:play()
    me "Hello World!"
    music:stop()
    sprite:hide()
    background:hide()

    -- jump to another registered label
    jump "goodbye"
    -- if you need you can call function as default
    -- jump("goodbye")
end


goodbye = Label("goodbye")
function goodbye:main()
    -- you can change text speed (0.05 is default speed)
    setTextSpeed(0.2)
    me "Goodbye World!"
end