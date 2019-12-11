Enumeration 
  #WIN_MAIN 
  
  ;different buttons
  #Button_1
  #Button_2
  #Button_3
  #Button_4
  #Button_5
  #Button_6
  #Button_7
  #Button_8
  #Button_9
  #Button_0
  #Button_Add
  #Button_Subtract
  #Button_Multiply
  #Button_Divide
  #Button_ReturnResult
  #Button_Period
  
  #BUTTON_CLEAR
  #DISPLAY_AREA
  #CONTAINER_GADGET
  
EndEnumeration

;again declaration of the behavior of the window
#F1 = #PB_Window_SystemMenu
#F2 = #PB_Window_ScreenCentered
#F3 = #PB_Window_MinimizeGadget

#Flags = #F1 | #F2 | #F3

;for the RunProgram()
#Flags2 = #PB_Program_Open | #PB_Program_Read | #PB_Program_Hide

;working directory
;please check this if there are problems
directory.s = GetCurrentDirectory()

If OpenWindow(#WIN_MAIN, 0, 0, 385, 500, "Simple Calculator", #Flags)  
  
  ;declarations of different widgets
  ;#PB_Default is Comic Sans MS                  
  LoadFont(1, "Comic Sans MS", 14,#PB_Font_Bold|#PB_Font_HighQuality)
  SetGadgetFont(#PB_Default,FontID(1))
  ButtonGadget(#BUTTON_CLEAR, 45, 135, 110,50, "CLEAR") 
  
  
  LoadFont(2, "Consolas", 18,#PB_Font_Bold|#PB_Font_HighQuality)
  SetGadgetFont(#PB_Default,FontID(2)) 
  
  ButtonGadget(#Button_1, 45, 200, 50, 50,"1")
  ButtonGadget(#Button_2, 105, 200, 50, 50,"2")
  ButtonGadget(#Button_3, 165, 200, 50, 50,"3")  
  ButtonGadget(#Button_Add, 225, 200, 50, 50,"+")
  ButtonGadget(#Button_Subtract, 285, 200, 50, 50,"-")
  
  ButtonGadget(#Button_4, 45, 270, 50, 50, "4")
  ButtonGadget(#Button_5, 105, 270, 50, 50, "5")
  ButtonGadget(#Button_6, 165, 270, 50, 50, "6")  
  ButtonGadget(#Button_Multiply, 225, 270, 50, 50, "*")
  ButtonGadget(#Button_Divide, 285, 270, 50, 50,"/")
  
  ButtonGadget(#Button_7, 45, 340, 50, 50, "7")
  ButtonGadget(#Button_8, 105, 340, 50, 50, "8")  
  ButtonGadget(#Button_9, 165, 340, 50, 50, "9")
  ButtonGadget(#Button_ReturnResult, 225, 340, 110, 120, "=")
  
  ButtonGadget(#Button_0, 45, 410, 110, 50, "0") 
  ButtonGadget(#Button_Period, 165, 410, 50, 50, Chr(46))
  
  ContainerGadget(#CONTAINER_GADGET, 15, 10, 355, 110, #PB_Container_Double)
  SetGadgetColor(#CONTAINER_GADGET, #PB_Gadget_BackColor, RGB(255,255,255)) 
  LoadFont(3, "Consolas", 20,#PB_Font_HighQuality)
  SetGadgetFont(#PB_Default,FontID(3))
  TextGadget(#DISPLAY_AREA, 0, 35, 345,28,"", #PB_Text_Right) 
  SetGadgetColor(#DISPLAY_AREA, #PB_Gadget_BackColor, RGB(255,255,255))  
  
  Repeat 
    Event.l = WaitWindowEvent()
    Select Event
      Case  #PB_Event_Gadget
        
        Select EventGadget()    
            
          Case #BUTTON_1           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_1)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_2           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_2)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_3           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_3)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_4           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_4)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_5           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_5)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_6           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_6)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_7           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_7)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_8           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_8)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_9           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_9)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_0           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_0)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #Button_Period           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_Period)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #Button_Add           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_Add)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_Subtract          
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_Subtract)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_Multiply           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_Multiply)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_Divide           
            old.s = GetGadgetText(#DISPLAY_AREA)
            new.s = GetGadgetText(#Button_Divide)
            SetGadgetText(#DISPLAY_AREA, old + new)
            
          Case #BUTTON_CLEAR
            SetGadgetText(#DISPLAY_AREA, "")
            
            DisableGadget(#Button_0,0)
            DisableGadget(#Button_1,0)
            DisableGadget(#Button_2,0)
            DisableGadget(#Button_3,0)
            DisableGadget(#Button_4,0)
            DisableGadget(#Button_5,0)
            DisableGadget(#Button_6,0)
            DisableGadget(#Button_7,0)
            DisableGadget(#Button_8,0)
            DisableGadget(#Button_9,0)
            DisableGadget(#Button_Add,0)
            DisableGadget(#Button_Subtract,0)
            DisableGadget(#Button_Multiply,0)
            DisableGadget(#Button_Divide,0)
            DisableGadget(#Button_Period,0)
            DisableGadget(#Button_ReturnResult,0)
                                    
          Case #Button_ReturnResult
            argument.s = GetGadgetText(#DISPLAY_AREA)
            SimpleCalculator = RunProgram( directory + "\calc.exe", argument, directory, #Flags2)
            ;Debug SimpleCalculator
            result.s = argument + "=" + ReadProgramString(SimpleCalculator)
            SetGadgetText(#DISPLAY_AREA, result)
            CloseProgram(SimpleCalculator)
            
            DisableGadget(#Button_0,1)
            DisableGadget(#Button_1,1)
            DisableGadget(#Button_2,1)
            DisableGadget(#Button_3,1)
            DisableGadget(#Button_4,1)
            DisableGadget(#Button_5,1)
            DisableGadget(#Button_6,1)
            DisableGadget(#Button_7,1)
            DisableGadget(#Button_8,1)
            DisableGadget(#Button_9,1)
            DisableGadget(#Button_Add,1)
            DisableGadget(#Button_Subtract,1)
            DisableGadget(#Button_Multiply,1)
            DisableGadget(#Button_Divide,1)
            DisableGadget(#Button_Period,1)
            DisableGadget(#Button_ReturnResult,1)
            
        EndSelect
    EndSelect
    
  Until Event = #PB_Event_CloseWindow 
EndIf
