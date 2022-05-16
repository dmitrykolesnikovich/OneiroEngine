-- creates characters, sprites, and other means
me = Character("Me")
-- second parameter is aspect ratio for sprite
-- for normal resizing sprite to window size you need keep aspect ratio
-- but in background you dont need to enabling aspect ratio
sprite = Sprite2D("Assets/Textures/sprite.png", true)
music = Audio("Assets/Audio/music.mp3")

-- you must to set the var name that was passed to the label ctor
-- because for call to label function engine need know name of variable
start = Label("start")
-- entry point in label is main function
-- you can use colon when you call or define function
function start:main()
    sprite:show()
    music:play()
    me "Hello World!"
    music:stop()
    sprite:hide()

    -- jump to another registered label
    jump "goodbye"
    -- if you need you can call function as default
    -- jump("goodbye")
end


goodbye = Label("goodbye")
function goodbye:main()
    me "Goodbye World!"
end