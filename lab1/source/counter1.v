`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/12/2025 11:39:16 AM
// Design Name: 
// Module Name: counter1
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module counter1(CLOCK,RES,BUTTONS,LEDS);
    input CLOCK;
    input RES;
    input [1:0] BUTTONS;
    output [3:0] LEDS;
    reg [31:0] clockc = 0;
    reg [3:0] count = 4'b0000;
    reg clk1hz =0;
    
    always@ (posedge(CLOCK))
        
        begin
            clockc <= clockc + 1;
            
            if (clockc == 62500000) begin
                
                clk1hz <= ~clk1hz;
                clockc <= 0;   
                
             end
             
         end  
    
    
    always @( posedge(clk1hz) )begin
        if (RES)begin
            count <= 4'b0000;
        end
    
        else if (BUTTONS[0]) begin
            count <= count + 1;
            end
        else if (BUTTONS[1]) begin
            count <= count - 1;
            end
        
        end
        
   assign LEDS[3:0]= count[3:0];
        
endmodule
