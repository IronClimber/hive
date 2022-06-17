import {
    AppBar,
    Toolbar,
    Typography,
  } from "@mui/material";

export function Header() {
    return (
        <AppBar position="static">
        <Toolbar>
          <Typography variant="h3">
            Project Hive
          </Typography>
        </Toolbar>
      </AppBar>
    )
}