import { Button, TextField } from "@mui/material"
import { Box } from "@mui/system"
import { useState } from "react"

export const Buttons = () => {
    const [period, setPeriod] = useState("")
    const [duration, setDuration] = useState("")

    const handleSetPeriod = () => {
        // fetch ....
        setPeriod("")
    }

    const handleSetDuration = () => {
        // fetch ....
        setDuration("")
    }

    return (
        <>
        <Box sx={{
            display: "flex",
            alignItems: "center"
        }}>
            <TextField 
                value={period}
                onChange={(event) => setPeriod(event.target.value)}
            />
            <Button variant="contained" onClick={handleSetPeriod}>
                Set period
            </Button>
        </Box>
        <Box sx={{
            display: "flex",
            alignItems: "center"
        }}>
            <TextField 
                value={duration}
                onChange={(event) => setDuration(event.target.value)}
            />
            <Button variant="contained" onClick={handleSetDuration}>
                Set duration
            </Button>
        </Box>
        </>
    )
}