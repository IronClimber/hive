import { Box, Button, Typography } from "@mui/material";
import { FlashOn, FlashOff } from "@mui/icons-material";
import { useQuery } from 'react-query'

import { Buttons } from "./Buttons";
import { getDataAPI } from '../api'
import { Charts } from "./Charts";

export function ControlPanel() {
    const { data, isLoading, error } = useQuery('data', getDataAPI)

    const handleReset = () => {
        console.log("reset")
        // fetch("url", {
        //     method: "POST"
        // })
    }

    if (isLoading || !data) {
        return (
            <Typography variant="h2">
                LOADING...
            </Typography>
        )
    }

    if (error) {
        return (
            <Typography variant="h2">
                Something went wrong
            </Typography>
        )
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
                    data[0].fields.period
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
                    Period: {data[0].fields.period}
                </Typography>
                <Typography marginX={1}>
                    Duration: {data[0].fields.duration}
                </Typography>
            </Box>

            <Buttons />
            <Charts data={data} />
        </Box>
    )
}
