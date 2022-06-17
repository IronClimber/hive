import { Box, Button, Typography } from "@mui/material";
import { FlashOn, FlashOff } from "@mui/icons-material";
import { useEffect, useState } from "react";
import { Buttons } from "./Buttons";

export function ControlPanel() {
    const [fanMode, setFanMode] = useState(false); // destruction
    const period = 60;
    const duration = 15;

    useEffect(() => {
        // fetch("https://jsonplaceholder.typicode.com/posts/1")
        //     .then((response) => response.json())
        //     .then((json) => console.log(json)); test
        //     .then((json) => set...(json));
        setTimeout(() => {
            setFanMode(true)
        }, 5000)
    }, [])

    const handleReset = () => {
        console.log("reset")
        setFanMode(false)
        // fetch("url", {
        //     method: "POST"
        // })
    }

    return (
        <Box sx={{
            padding: 1
        }}>
            <Box sx={{
                display: "flex",
                alignItems: "center"
            }}>
                {
                    fanMode
                        ? <FlashOn color="success"/>
                        : <FlashOff color="error"/>
                }
                <Button
                    variant="contained"
                    onClick={handleReset}
                    sx={{ marginLeft: 1}}>
                    Reset
                </Button>
                <Typography marginX={1}>
                    Period: {period}
                </Typography>
                <Typography marginX={1}>
                    Duration: {duration}
                </Typography>
            </Box>
            <Buttons />
        </Box>
    )
}
